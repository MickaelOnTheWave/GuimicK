#include "windowsscheduler.h"

#include <comdef.h>

namespace
{
   wchar_t* defaultTaskName = L"TaskManager Automated Task";
   wchar_t* defaultAuthor = L"Task Manager";
   wchar_t* defaultDescription = L"This task runs periodically TaskManager.\n"
                                 L"It was created automatically by the Configuration editor.";
   wchar_t* defaultTriggerName = L"Default Trigger";

   TASK_TRIGGER_TYPE2 TranslateTriggerType(ScheduleData* data)
   {
      if (dynamic_cast<ScheduleDailyData*>(data))
         return TASK_TRIGGER_DAILY;
      else if (dynamic_cast<ScheduleWeeklyData*>(data))
         return TASK_TRIGGER_WEEKLY;
      else if (dynamic_cast<ScheduleMonthlyData*>(data))
         return TASK_TRIGGER_MONTHLY;
      else
         return TASK_TRIGGER_EVENT; // TODO : change this!
   }

   BSTR GetCurrentTimeString()
   {
      SYSTEMTIME currentTime;
      GetLocalTime(&currentTime);

      const size_t bufferSize = 100;
      wchar_t buffer[bufferSize];
      swprintf_s(buffer, bufferSize, L"%4d-%02d-%02dT%02d:%02d:%02d", 
                 currentTime.wYear, currentTime.wMonth, currentTime.wDay,
                 currentTime.wHour, currentTime.wMinute, currentTime.wSecond);
      return _bstr_t(buffer);
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

   if (comInitialized)
      CoUninitialize();
}

ScheduleData* WindowsScheduler::Read() const
{
   if (comInitialized && winApiAvailable)
   {
      ITaskFolder* taskRootFolder = GetTaskRootFolder();
      if (taskRootFolder)
      {
         IRegisteredTask* task = NULL;
         taskRootFolder->GetTask(defaultTaskName, &task);
         if (task)
            return CreateDataFromTask(task);
      }
   }

   return nullptr;
}

bool WindowsScheduler::Write(ScheduleData* data)
{
   bool result = false;
   if (comInitialized && winApiAvailable)
   {
      ITaskFolder* taskRootFolder = GetTaskRootFolder();
      if (taskRootFolder)
      {
         taskRootFolder->DeleteTask(defaultTaskName, 0);
         ITaskDefinition* task = CreateTaskFromData(data);
         if (task)
         {
            result = RegisterTask(taskRootFolder, task);
            task->Release();
         }
         taskRootFolder->Release();
      }
   }
   return result;
}

void WindowsScheduler::InitializeWindowsApi()
{
   InitializeCom();
   InitializeComSecurity();
   CreateTaskService();
}

void WindowsScheduler::InitializeCom()
{
   //HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
   //comInitialized = SUCCEEDED(hr);
   comInitialized = true;
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
      UpdateLastErrorMessage(hr);
   return ok;
}

ScheduleData* WindowsScheduler::CreateDataFromTask(IRegisteredTask* task) const
{
   return nullptr;
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

   UpdateLastErrorMessage(hr);
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

   UpdateLastErrorMessage(hr);
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
         bool ok = SetCommonTriggerData(taskTrigger);
         if (ok)
            ok = SetTypedTriggerData(taskTrigger, data);
         taskTrigger->Release();
      }
      triggerCollection->Release();
   }

   UpdateLastErrorMessage(hr);
   return SUCCEEDED(hr);
}

bool WindowsScheduler::SetCommonTriggerData(ITrigger* trigger)
{
   HRESULT hr = trigger->put_Id(defaultTriggerName);
   if (SUCCEEDED(hr))
      hr = trigger->put_StartBoundary(GetCurrentTimeString());

   UpdateLastErrorMessage(hr);
   return SUCCEEDED(hr);
}

bool WindowsScheduler::SetTypedTriggerData(ITrigger* trigger, ScheduleData* data)
{
   if (dynamic_cast<ScheduleDailyData*>(data))
      return SetDailyTriggerData(trigger, data);
   else if (dynamic_cast<ScheduleWeeklyData*>(data))
      return SetWeeklyTriggerData(trigger, data);
   else if (dynamic_cast<ScheduleMonthlyData*>(data))
      return SetMontlyTriggerData(trigger, data);
   else
      return true;
}

bool WindowsScheduler::SetDailyTriggerData(ITrigger* trigger, ScheduleData* data)
{
   IDailyTrigger *dailyTrigger = nullptr;
   HRESULT hr = trigger->QueryInterface(IID_IDailyTrigger, (void**)&dailyTrigger);
   if (SUCCEEDED(hr))
   {
      hr = dailyTrigger->put_DaysInterval((short)1);
      dailyTrigger->Release();
   }
   return SUCCEEDED(hr);
}

bool WindowsScheduler::SetWeeklyTriggerData(ITrigger* trigger, ScheduleData* data)
{
   IWeeklyTrigger *pWeeklyTrigger = NULL;
   HRESULT hr = trigger->QueryInterface(
      IID_IWeeklyTrigger, (void**)&pWeeklyTrigger);

   if (SUCCEEDED(hr))
   {
      hr = pWeeklyTrigger->put_WeeksInterval((short)1);
      if (SUCCEEDED(hr))
         hr = pWeeklyTrigger->put_DaysOfWeek((short)2);

      pWeeklyTrigger->Release();
   }
   return SUCCEEDED(hr);
}

bool WindowsScheduler::SetMontlyTriggerData(ITrigger* trigger, ScheduleData* data)
{
   IMonthlyTrigger *monthlyTrigger = nullptr;
   HRESULT hr = trigger->QueryInterface(IID_IMonthlyTrigger, (void**)&monthlyTrigger);
   if (SUCCEEDED(hr))
   {
      hr = monthlyTrigger->put_DaysOfMonth(5);
      monthlyTrigger->Release();
   }
   return SUCCEEDED(hr);
}

bool WindowsScheduler::SetTaskAction(ITaskDefinition* taskDefinition)
{
   IActionCollection *pActionCollection = NULL;
   HRESULT hr = taskDefinition->get_Actions(&pActionCollection);
   if (SUCCEEDED(hr))
   {
      IAction *pAction = NULL;
      hr = pActionCollection->Create(TASK_ACTION_EXEC, &pAction);
      if (SUCCEEDED(hr))
      {
         IExecAction *pExecAction = NULL;
         //  QI for the executable task pointer.
         hr = pAction->QueryInterface(IID_IExecAction, (void**)&pExecAction);
         if (SUCCEEDED(hr))
         {
            std::wstring wstrExecutablePath = _wgetenv(L"WINDIR");
            wstrExecutablePath += L"\\SYSTEM32\\NOTEPAD.EXE";
            hr = pExecAction->put_Path(_bstr_t(wstrExecutablePath.c_str()));
         }
      }

   }

   UpdateLastErrorMessage(hr);
   return SUCCEEDED(hr);
}

void WindowsScheduler::UpdateLastErrorMessage(const HRESULT hr)
{
   _com_error comError(hr);
   const LPCTSTR errorMsg = comError.ErrorMessage();
   lastError = errorMsg;
}
