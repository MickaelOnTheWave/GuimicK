#ifndef WINDOWSSCHEDULER_H
#define WINDOWSSCHEDULER_H

#include "abstractscheduler.h"

#include <taskschd.h>

class WindowsScheduler : public AbstractScheduler
{
public:
   WindowsScheduler();
   ~WindowsScheduler();

   bool Read(ScheduleTarget &data) override;
   bool Write(const ScheduleTarget &data) override;

private:
   void InitializeWindowsApi();
   void InitializeCom();
   void InitializeComSecurity();
   void CreateTaskService();

   ITaskFolder* GetTaskRootFolder();

   void FillDataFromTask(IRegisteredTask* task,
                         ScheduleTarget& data);
   ITaskDefinition* CreateTaskFromData(const ScheduleTarget& data);

   bool comInitialized = false;
   bool winApiAvailable = true;
   bool taskServiceAvailable = true;
   ITaskService* taskService = nullptr;
};

#endif // WINDOWSSCHEDULER_H
