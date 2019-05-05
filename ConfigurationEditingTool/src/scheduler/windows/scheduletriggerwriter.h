#ifndef SCHEDULETRIGGERWRITER_H
#define SCHEDULETRIGGERWRITER_H

#include <taskschd.h>
#include "scheduledata.h"
#include "schedulererrormanager.h"

class ScheduleTriggerWriter
{
public:
   ScheduleTriggerWriter(SchedulerErrorManager& _errorManager);
   virtual ~ScheduleTriggerWriter() = default;

   bool Write(ITrigger* trigger, ScheduleData* data);

private:
   bool SetCommonTriggerData(ITrigger* trigger,
                             ScheduleData* data);

   SchedulerErrorManager& errorManager;
};

#endif // SCHEDULETRIGGERWRITER_H
