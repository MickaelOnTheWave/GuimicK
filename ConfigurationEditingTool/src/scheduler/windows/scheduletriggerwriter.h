#ifndef SCHEDULETRIGGERWRITER_H
#define SCHEDULETRIGGERWRITER_H

#include <taskschd.h>
#include "scheduledata.h"
#include "windowsschedulererrormanager.h"

class ScheduleTriggerWriter
{
public:
   ScheduleTriggerWriter(WindowsSchedulerErrorManager& _errorManager);
   virtual ~ScheduleTriggerWriter() = default;

   bool Write(ITrigger* trigger, ScheduleData* data);

private:
   bool SetCommonTriggerData(ITrigger* trigger,
                             ScheduleData* data);

   WindowsSchedulerErrorManager& errorManager;
};

#endif // SCHEDULETRIGGERWRITER_H
