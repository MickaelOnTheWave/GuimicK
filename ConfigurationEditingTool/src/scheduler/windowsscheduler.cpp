#include "windowsscheduler.h"

#include <comdef.h>

namespace
{
   wchar_t* defaultTaskName = L"TaskManager Automated Task";
   wchar_t* defaultAuthor = L"Task Manager";

   TASK_TRIGGER_TYPE2 TranslateTriggerType(const ScheduleTarget::Type targetType)
   {
      if (targetType == ScheduleTarget::Type::Daily)
         return TASK_TRIGGER_DAILY;
      else if (targetType == ScheduleTarget::Type::Weekly)
         return TASK_TRIGGER_WEEKLY;
      else if (targetType == ScheduleTarget::Type::Monthly)
         return TASK_TRIGGER_MONTHLY;
      else
         return TASK_TRIGGER_EVENT; // TODO : change this!
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

bool WindowsScheduler::Read(ScheduleTarget& data)
{
   if (comInitialized && winApiAvailable)
   {
      ITaskFolder* taskRootFolder = GetTaskRootFolder();
      if (taskRootFolder)
      {
         IRegisteredTask* task = NULL;
         taskRootFolder->GetTask(defaultTaskName, &task);
         if (task)
            FillDataFromTask(task, data);
         return true;
      }
   }

   return false;
}

bool WindowsScheduler::Write(const ScheduleTarget& data)
{
   if (comInitialized && winApiAvailable)
   {
      ITaskFolder* taskRootFolder = GetTaskRootFolder();
      if (taskRootFolder)
      {
         taskRootFolder->DeleteTask(defaultTaskName, 0);
         ITaskDefinition* task = CreateTaskFromData(data);
         if (task)
         {
            IRegisteredTask* registeredTask = NULL;
            HRESULT hr = taskRootFolder->RegisterTaskDefinition(
               L"Auto Task Name", task, TASK_CREATE_OR_UPDATE,
               _variant_t(), _variant_t(), TASK_LOGON_NONE,
               _variant_t(L""), &registeredTask);

            return (SUCCEEDED(hr));
         }
      }
   }

   return false;
}

void WindowsScheduler::InitializeWindowsApi()
{
   // TODO : finish copying from docs.microsoft.com : weekly trigger example
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

ITaskFolder* WindowsScheduler::GetTaskRootFolder()
{
   ITaskFolder* taskRootFolder = nullptr;
   HRESULT hr = taskService->GetFolder(L"", &taskRootFolder);
   return (SUCCEEDED(hr)) ? taskRootFolder : nullptr;
}

void WindowsScheduler::FillDataFromTask(IRegisteredTask* task,
                                        ScheduleTarget& data)
{
}

ITaskDefinition* WindowsScheduler::CreateTaskFromData(const ScheduleTarget& data)
{
   ITaskDefinition* taskDefinition = NULL;
   HRESULT hr = taskService->NewTask(0, &taskDefinition);
   if (SUCCEEDED(hr))
   {
      IRegistrationInfo *pRegInfo = NULL;
      hr = taskDefinition->get_RegistrationInfo(&pRegInfo);
      if (SUCCEEDED(hr))
      {
         hr = pRegInfo->put_Author(defaultAuthor);
         if (SUCCEEDED(hr))
         {
            ITriggerCollection *pTriggerCollection = NULL;
            hr = taskDefinition->get_Triggers(&pTriggerCollection);
            if (SUCCEEDED(hr))
            {
               if (data.type != ScheduleTarget::Type::Unset)
               {
                  TASK_TRIGGER_TYPE2 triggerType = TranslateTriggerType(data.type);
                  ITrigger *pTrigger = NULL;
                  hr = pTriggerCollection->Create(triggerType, &pTrigger);
                  if (SUCCEEDED(hr))
                  {
                     hr = pTrigger->put_Id(defaultTaskName);
                     if (SUCCEEDED(hr))
                     {
                        // TODO : continue

                     }
                     /*IWeeklyTrigger *pWeeklyTrigger = NULL;
                     hr = pTrigger->QueryInterface(
                        IID_IWeeklyTrigger, (void**)&pWeeklyTrigger);*/
                  }
               }
            }
         }
      }
   }
   return taskDefinition;
}
