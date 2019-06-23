#ifndef WINDOWSSCHEDULER_H
#define WINDOWSSCHEDULER_H

#include "abstractscheduler.h"

#include <taskschd.h>
#include "windowsschedulererrormanager.h"

class WindowsScheduler : public AbstractScheduler
{
public:
   WindowsScheduler();
   virtual ~WindowsScheduler();

   bool Read(ScheduleData** data) const override;
   bool Write(ScheduleData* data) override;
   QString GetLastError() const override;

private:
   void InitializeWindowsApi();
   void InitializeComSecurity();
   void CreateTaskService();

   ITaskFolder* GetTaskRootFolder() const;

   bool RegisterTask(ITaskFolder* taskFolder,
                     ITaskDefinition* taskDefinition);

   ScheduleData* CreateDataFromTask(IRegisteredTask* task) const;

   ITaskDefinition* CreateTaskFromData(ScheduleData* data);

   bool SetTaskRegistrationData(ITaskDefinition* taskDefinition);
   bool SetTaskTriggerData(ITaskDefinition* taskDefinition,
                           ScheduleData* data);
   bool SetTaskAction(ITaskDefinition* taskDefinition);
   bool SetTaskSecuritySettings(ITaskDefinition* taskDefinition);
   bool SetTaskOtherSettings(ITaskDefinition* taskDefinition);

   bool winApiAvailable = true;
   bool taskServiceAvailable = true;
   ITaskService* taskService = nullptr;
   WindowsSchedulerErrorManager errorManager;
};

#endif // WINDOWSSCHEDULER_H
