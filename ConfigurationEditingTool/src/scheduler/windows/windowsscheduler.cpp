#include "windowsscheduler.h"

#include <comdef.h>
#include "scheduletriggerreader.h"
#include "scheduletriggerwriter.h"

namespace
{
   wchar_t* defaultTaskName = L"TaskManager Automated Task";
   wchar_t* defaultAuthor = L"Task Manager";
   wchar_t* defaultDescription = L"This task runs periodically TaskManager.\n"
                                 L"It was created automatically by the Configuration editor.";

   TASK_TRIGGER_TYPE2 TranslateTriggerType(ScheduleData* data)
   {
      if (dynamic_cast<ScheduleDailyData*>(data))
         return TASK_TRIGGER_DAILY;
      else if (dynamic_cast<ScheduleMonthlyData*>(data))
         return TASK_TRIGGER_MONTHLY;
      else if (dynamic_cast<ScheduleWeeklyData*>(data))
         return TASK_TRIGGER_WEEKLY;
      else
         return TASK_TRIGGER_IDLE;
   }
}

WindowsScheduler::WindowsScheduler()
{
   InitializeWindowsApi();
}

WindowsScheduler::~WindowsScheduler()
{
   if (taskServiceAvailable)
      taskService->Release();
}

bool WindowsScheduler::Read(ScheduleData** data) const
{
   if (winApiAvailable)
   {
      ITaskFolder* taskRootFolder = GetTaskRootFolder();
      if (taskRootFolder)
      {
         IRegisteredTask* task = NULL;
         taskRootFolder->GetTask(defaultTaskName, &task);
         if (task)
            *data = CreateDataFromTask(task);
         return true;
      }
   }
   return false;
}

bool WindowsScheduler::Write(ScheduleData* data)
{
   bool result = false;
   if (winApiAvailable)
   {
      ITaskFolder* taskRootFolder = GetTaskRootFolder();
      if (taskRootFolder)
      {
         taskRootFolder->DeleteTask(defaultTaskName, 0);
         if (data)
         {
            ITaskDefinition* task = CreateTaskFromData(data);
            if (task)
            {
               result = RegisterTask(taskRootFolder, task);
               task->Release();
            }
         }
         else
            result = true;
         taskRootFolder->Release();
      }
   }
   return result;
}

void WindowsScheduler::InitializeWindowsApi()
{
   InitializeComSecurity();
   CreateTaskService();
}

void WindowsScheduler::InitializeComSecurity()
{
   HRESULT hr = CoInitializeSecurity(NULL, -1, NULL, NULL, 
                                     RPC_C_AUTHN_LEVEL_PKT,
                                     RPC_C_IMP_LEVEL_IMPERSONATE,
                                     NULL, 0, NULL);
   winApiAvailable = SUCCEEDED(hr);
}

void WindowsScheduler::CreateTaskService()
{
   HRESULT hr = CoCreateInstance(CLSID_TaskScheduler, NULL,
                                 CLSCTX_INPROC_SERVER,
                                 IID_ITaskService, (void**)&taskService);
   if (SUCCEEDED(hr))
   {
      hr = taskService->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());
      taskServiceAvailable = SUCCEEDED(hr);
   }

   winApiAvailable = taskServiceAvailable;
}

ITaskFolder* WindowsScheduler::GetTaskRootFolder() const
{
   ITaskFolder* taskRootFolder = nullptr;
   HRESULT hr = taskService->GetFolder(L"", &taskRootFolder);
   return (SUCCEEDED(hr)) ? taskRootFolder : nullptr;
}

bool WindowsScheduler::RegisterTask(ITaskFolder* taskFolder,
                                    ITaskDefinition* taskDefinition)
{
   IRegisteredTask* registeredTask = NULL;
   const HRESULT hr = taskFolder->RegisterTaskDefinition(
      defaultTaskName, taskDefinition,
      TASK_CREATE_OR_UPDATE, _variant_t(), _variant_t(),
      TASK_LOGON_NONE, _variant_t(L""), &registeredTask
   );

   const bool ok = SUCCEEDED(hr);
   if (ok)
      registeredTask->Release();
   else
      errorManager.UpdateLastErrorMessage(hr);
   return ok;
}

ScheduleData* WindowsScheduler::CreateDataFromTask(IRegisteredTask* task) const
{
   ScheduleData* scheduleData = nullptr;
   ITaskDefinition* taskDefinition = nullptr;
   HRESULT hr = task->get_Definition(&taskDefinition);
   if (SUCCEEDED(hr))
   {
      ITriggerCollection* taskTriggers = nullptr;
      hr = taskDefinition->get_Triggers(&taskTriggers);
      if (SUCCEEDED(hr))
      {
         long triggerCount = 0;
         hr = taskTriggers->get_Count(&triggerCount);
         if (SUCCEEDED(hr) && triggerCount > 0)
         {
            ITrigger* trigger = nullptr;
            taskTriggers->get_Item(1, &trigger);
            if (SUCCEEDED(hr))
            {
               scheduleData = ScheduleTriggerReader::Read(trigger);
               trigger->Release();
            }
         }
         taskTriggers->Release();
      }
      taskDefinition->Release();
   }
   return scheduleData;
}

ITaskDefinition* WindowsScheduler::CreateTaskFromData(ScheduleData* data)
{
   ITaskDefinition* taskDefinition = nullptr;
   HRESULT hr = taskService->NewTask(0, &taskDefinition);
   if (SUCCEEDED(hr))
   {
      bool ok = SetTaskRegistrationData(taskDefinition);
      if (ok)
         ok = SetTaskTriggerData(taskDefinition, data);

      if (ok)
         ok = SetTaskAction(taskDefinition);
   }

   errorManager.UpdateLastErrorMessage(hr);
   return taskDefinition;
}

bool WindowsScheduler::SetTaskRegistrationData(ITaskDefinition* taskDefinition)
{
   IRegistrationInfo *taskRegistrationInfo = nullptr;
   HRESULT hr = taskDefinition->get_RegistrationInfo(&taskRegistrationInfo);
   if (SUCCEEDED(hr))
   {
      hr = taskRegistrationInfo->put_Author(defaultAuthor);
      if (SUCCEEDED(hr))
         hr = taskRegistrationInfo->put_Description(defaultDescription);

      taskRegistrationInfo->Release();
   }

   errorManager.UpdateLastErrorMessage(hr);
   return SUCCEEDED(hr);
}

bool WindowsScheduler::SetTaskTriggerData(ITaskDefinition* taskDefinition,
                                          ScheduleData* data)
{
   ITriggerCollection *triggerCollection = nullptr;
   HRESULT hr = taskDefinition->get_Triggers(&triggerCollection);
   if (SUCCEEDED(hr))
   {
      TASK_TRIGGER_TYPE2 triggerType = TranslateTriggerType(data);
      ITrigger *taskTrigger = nullptr;
      hr = triggerCollection->Create(triggerType, &taskTrigger);
      if (SUCCEEDED(hr))
      {
         ScheduleTriggerWriter writer(errorManager);
         writer.Write(taskTrigger, data);
         taskTrigger->Release();
      }
      triggerCollection->Release();
   }

   errorManager.UpdateLastErrorMessage(hr);
   return SUCCEEDED(hr);
}

bool WindowsScheduler::SetTaskAction(ITaskDefinition* taskDefinition)
{
   IActionCollection *pActionCollection = NULL;
   HRESULT hr = taskDefinition->get_Actions(&pActionCollection);
   if (SUCCEEDED(hr))
   {
      IAction *taskAction = NULL;
      hr = pActionCollection->Create(TASK_ACTION_EXEC, &taskAction);
      if (SUCCEEDED(hr))
      {
         IExecAction *taskExeAction = NULL;
         hr = taskAction->QueryInterface(IID_IExecAction, (void**)&taskExeAction);
         if (SUCCEEDED(hr))
         {
            hr = taskExeAction->put_Path(_bstr_t(commandToRun.toStdWString().c_str()));

            if (SUCCEEDED(hr))
               hr = taskExeAction->put_Arguments(_bstr_t(commandArguments.toStdWString().c_str()));

            taskExeAction->Release();
         }
         taskAction->Release();
      }
   }

   errorManager.UpdateLastErrorMessage(hr);
   return SUCCEEDED(hr);
}
