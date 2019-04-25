#ifndef ABSTRACTSCHEDULER_H
#define ABSTRACTSCHEDULER_H

#include <QString>
#include "scheduledata.h"

class AbstractScheduler
{
public:
   virtual ScheduleData* Read() const = 0;
   virtual bool Write(ScheduleData* data) = 0;

   QString GetLastError() const;
   void SetCommandToRun(const QString& value);
   void SetCommandArguments(const QString& value);

protected:
   QString lastError = "";
   QString commandToRun = "";
   QString commandArguments = "";
};

#endif // ABSTRACTSCHEDULER_H
