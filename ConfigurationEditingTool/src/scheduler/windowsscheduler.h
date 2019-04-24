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

   bool RegisterTask(ITaskFolder* taskFolder,
                     ITaskDefinition* taskDefinition);

   void FillDataFromTask(IRegisteredTask* task,
                         ScheduleTarget& data);
   ITaskDefinition* CreateTaskFromData(const ScheduleTarget& data);

   bool SetTaskRegistrationData(ITaskDefinition* taskDefinition);
   bool SetTaskTriggerData(ITaskDefinition* taskDefinition,
                           const ScheduleTarget& data);
   bool SetCommonTriggerData(ITrigger* trigger);
   bool SetTypedTriggerData(ITrigger* trigger,
                            const ScheduleTarget& data);
   bool SetDailyTriggerData(ITrigger* trigger,
                            const ScheduleTarget& data);
   bool SetWeeklyTriggerData(ITrigger* trigger,
                            const ScheduleTarget& data);
   bool SetMontlyTriggerData(ITrigger* trigger,
                             const ScheduleTarget& data);
   bool SetTaskAction(ITaskDefinition* taskDefinition);

   void UpdateLastErrorMessage(const HRESULT hr);

   bool comInitialized = false;
   bool winApiAvailable = true;
   bool taskServiceAvailable = true;
   ITaskService* taskService = nullptr;
};

#endif // WINDOWSSCHEDULER_H
