#ifndef SCHEDULETRIGGERREADER_H
#define SCHEDULETRIGGERREADER_H

#include <taskschd.h>
#include "scheduledata.h"

namespace ScheduleTriggerReader
{
   ScheduleData* Read(ITrigger* trigger);
}

#endif // SCHEDULETRIGGERREADER_H
