#ifndef ABSTRACTSCHEDULER_H
#define ABSTRACTSCHEDULER_H

#include <QString>
#include "scheduledata.h"

class AbstractScheduler
{
public:
   virtual ScheduleData* Read() const = 0;
   virtual bool Write(ScheduleData* data) = 0;
   QString GetLastError() const
   {
      return lastError;
   };

protected:
   QString lastError = "";
};

#endif // ABSTRACTSCHEDULER_H
