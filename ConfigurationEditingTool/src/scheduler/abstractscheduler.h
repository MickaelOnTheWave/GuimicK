#ifndef ABSTRACTSCHEDULER_H
#define ABSTRACTSCHEDULER_H

#include <QString>
#include "scheduledata.h"

class ScheduleTarget
{
public:
    enum class Type {Unset, Daily, Weekly, Monthly};

    Type type = Type::Unset;
    QString details = "";
};

class AbstractScheduler
{
public:
   virtual bool Read(ScheduleTarget& data) = 0;
   virtual bool Write(ScheduleData* data) = 0;
   QString GetLastError() const
   {
      return lastError;
   };

protected:
   QString lastError = "";
};

#endif // ABSTRACTSCHEDULER_H
