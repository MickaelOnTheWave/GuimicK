#include "windowsscheduler.h"

#include <comdef.h>
#include "scheduletriggerreader.h"
#include "scheduletriggerwriter.h"
#include "windowscredentialsmanager.h"
#include "windowsschedulererrormanager.h"

#include <wbemidl.h>
#include <wincred.h>

namespace
{
   const wchar_t* defaultTaskName = L"TaskManager Automated Task";
   const wchar_t* defaultAuthor = L"Task Manager";
   const wchar_t* defaultDescription = L"This task runs periodically TaskManager. \n"
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

   _variant_t ToVariant(const std::string& input)
   {
       _variant_t vt;
       _bstr_t bs(input.c_str());
       reinterpret_cast<_variant_t&>(vt) = bs;
       return vt;
   }

   HRESULT RegisterTask(
               IRegisteredTask** registeredTask,
               const Credentials& credentials,
               ITaskFolder* taskFolder, ITaskDefinition* taskDefinition,
           TASK_LOGON_TYPE logonType)
   {
       const HRESULT hr = taskFolder->RegisterTaskDefinition(
          _bstr_t(defaultTaskName), taskDefinition,
          TASK_CREATE_OR_UPDATE,
          ToVariant(credentials.GetDomainUsername()),
          ToVariant(credentials.password),
          logonType,
          _variant_t(L""),
          registeredTask
       );
       return hr;
   }

   HRESULT RegisterTaskAsOutOfSession(
               IRegisteredTask** registeredTask,
               const Credentials& credentials,
               ITaskFolder* taskFolder, ITaskDefinition* taskDefinition)
   {
       return RegisterTask(registeredTask,
                           credentials,
                           taskFolder, taskDefinition,
                           TASK_LOGON_INTERACTIVE_TOKEN_OR_PASSWORD);
   }

   HRESULT RegisterTaskOnlyInsideSession(
               IRegisteredTask** registeredTask,
               ITaskFolder* taskFolder, ITaskDefinition* taskDefinition)
   {
       Credentials emptyCredentials;
       return RegisterTask(registeredTask,
                           emptyCredentials,
                           taskFolder, taskDefinition,
                           TASK_LOGON_NONE);
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
         IRegisteredTask* task = nullptr;
         taskRootFolder->GetTask(_bstr_t(defaultTaskName), &task);
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
         taskRootFolder->DeleteTask(_bstr_t(defaultTaskName), 0);
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

QString WindowsScheduler::GetLastError() const
{
   return errorManager.GetLastError();
}

void WindowsScheduler::InitializeWindowsApi()
{
   InitializeComSecurity();
   CreateTaskService();
}

void WindowsScheduler::InitializeComSecurity()
{
   HRESULT hr = CoInitializeSecurity(nullptr, -1, nullptr, nullptr,
                                     RPC_C_AUTHN_LEVEL_PKT,
                                     RPC_C_IMP_LEVEL_IMPERSONATE,
                                     nullptr, 0, nullptr);
   winApiAvailable = SUCCEEDED(hr);
}

void WindowsScheduler::CreateTaskService()
{
   HRESULT hr = CoCreateInstance(CLSID_TaskScheduler, nullptr,
                                 CLSCTX_INPROC_SERVER,
                                 IID_ITaskService,
                                 reinterpret_cast<void**>(&taskService));
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
   HRESULT hr = taskService->GetFolder(_bstr_t(L""), &taskRootFolder);
   return (SUCCEEDED(hr)) ? taskRootFolder : nullptr;
}

bool WindowsScheduler::RegisterTask(ITaskFolder* taskFolder,
                                    ITaskDefinition* taskDefinition)
{
   IRegisteredTask* registeredTask = nullptr;

   WindowsCredentialsManager credentialsManager;
   HRESULT result = credentialsManager.AskUser();
   if (SUCCEEDED(result))
   {
       result = RegisterTaskAsOutOfSession(
                   &registeredTask,
                   credentialsManager.GetCredentials(),
                   taskFolder, taskDefinition
                   );
   }
   else
       result = RegisterTaskOnlyInsideSession(&registeredTask, taskFolder, taskDefinition);

   const bool ok = SUCCEEDED(result);
   if (ok)
      registeredTask->Release();
   else
      errorManager.UpdateLastErrorMessage(result);
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

      if (ok)
          ok = SetTaskSecuritySettings(taskDefinition);

      if (ok)
          ok = SetTaskOtherSettings(taskDefinition);
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
      hr = taskRegistrationInfo->put_Author(_bstr_t(defaultAuthor));
      if (SUCCEEDED(hr))
         hr = taskRegistrationInfo->put_Description(_bstr_t(defaultDescription));

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
   IActionCollection *pActionCollection = nullptr;
   HRESULT hr = taskDefinition->get_Actions(&pActionCollection);
   if (SUCCEEDED(hr))
   {
      IAction *taskAction = nullptr;
      hr = pActionCollection->Create(TASK_ACTION_EXEC, &taskAction);
      if (SUCCEEDED(hr))
      {
         IExecAction *taskExeAction = nullptr;
         hr = taskAction->QueryInterface(
                     IID_IExecAction,
                     reinterpret_cast<void**>(&taskExeAction));
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

bool WindowsScheduler::SetTaskSecuritySettings(ITaskDefinition* taskDefinition)
{
    IPrincipal* taskSecuritySettings = nullptr;
    HRESULT hr = taskDefinition->get_Principal(&taskSecuritySettings);
//    if (SUCCEEDED(hr))
//        hr = taskSecuritySettings->put_RunLevel(TASK_RUNLEVEL_HIGHEST);

    errorManager.UpdateLastErrorMessage(hr);
    return SUCCEEDED(hr);
}

bool WindowsScheduler::SetTaskOtherSettings(ITaskDefinition *taskDefinition)
{
    ITaskSettings* taskSettings = nullptr;
    HRESULT hr = taskDefinition->get_Settings(&taskSettings);
    if (SUCCEEDED(hr))
        hr = taskSettings->put_WakeToRun(TRUE);

    errorManager.UpdateLastErrorMessage(hr);
    return SUCCEEDED(hr);
}
