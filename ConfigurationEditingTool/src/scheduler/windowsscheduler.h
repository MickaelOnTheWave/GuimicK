#ifndef WINDOWSSCHEDULER_H
#define WINDOWSSCHEDULER_H

#include "abstractscheduler.h"

#include <taskschd.h>

class WindowsScheduler : public AbstractScheduler
{
public:
   WindowsScheduler();
   ~WindowsScheduler();

   bool Read(ScheduleData** data) const override;
   bool Write(ScheduleData* data) override;

private:
   void InitializeWindowsApi();
   void InitializeCom();
   void InitializeComSecurity();
   void CreateTaskService();

   ITaskFolder* GetTaskRootFolder() const;

   bool RegisterTask(ITaskFolder* taskFolder,
                     ITaskDefinition* taskDefinition);

   ScheduleData* CreateDataFromTask(IRegisteredTask* task) const;

   ScheduleData* CreateDataFromTrigger(ITrigger* trigger) const;
   ScheduleData* CreateDataFromDailyTrigger(ITrigger* trigger) const;
   ScheduleData* CreateDataFromWeeklyTrigger(ITrigger* trigger) const;
   ScheduleData* CreateDataFromMonthlyTrigger(ITrigger* trigger) const;

   ITaskDefinition* CreateTaskFromData(ScheduleData* data);

   bool SetTaskRegistrationData(ITaskDefinition* taskDefinition);
   bool SetTaskTriggerData(ITaskDefinition* taskDefinition,
                           ScheduleData* data);
   bool SetCommonTriggerData(ITrigger* trigger,
                             ScheduleData* data);
   bool SetTypedTriggerData(ITrigger* trigger,
                            ScheduleData* data);
   bool SetDailyTriggerData(ITrigger* trigger,
                            ScheduleDailyData* data);
   bool SetWeeklyTriggerData(ITrigger* trigger,
                             ScheduleWeeklyData* data);
   bool SetMontlyTriggerData(ITrigger* trigger,
                             ScheduleMonthlyData* data);
   bool SetTaskAction(ITaskDefinition* taskDefinition);

   void UpdateLastErrorMessage(const HRESULT hr);

   IDailyTrigger* GetDailyTrigger(ITrigger* trigger) const;

   IWeeklyTrigger* GetWeeklyTrigger(ITrigger* trigger) const;

   IMonthlyTrigger* GetMonthlyTrigger(ITrigger* trigger) const;

   void SetCommonDataFromTrigger(ScheduleData* data,
                                 ITrigger* trigger) const;
   void SetDayDataFromMask(ScheduleWeeklyData* data,
                           const long dayMask,
                           const long daysCount) const;

   bool winApiAvailable = true;
   bool taskServiceAvailable = true;
   ITaskService* taskService = nullptr;
};

#endif // WINDOWSSCHEDULER_H
