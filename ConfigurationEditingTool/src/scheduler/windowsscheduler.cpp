#include "windowsscheduler.h"

#include <comdef.h>

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
      return false;
   }
   else
      return false;
}

bool WindowsScheduler::Write(const ScheduleTarget& data)
{
   if (comInitialized && winApiAvailable)
   {
      return false;
   }
   else
      return false;
}

void WindowsScheduler::InitializeWindowsApi()
{
   // TODO : finish copying from docs.microsoft.com : weekly trigger example
   InitializeCom();
   InitializeComSecurity();
   CreateTaskService();
   //CreateTaskFolder();
   //DefineTaskInformation();
   //CreateTrigger();
   //CreateTaskAction();
}

void WindowsScheduler::InitializeCom()
{
   HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
   comInitialized = SUCCEEDED(hr);
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
