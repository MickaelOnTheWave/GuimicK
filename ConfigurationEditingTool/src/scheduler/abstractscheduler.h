#ifndef ABSTRACTSCHEDULER_H
#define ABSTRACTSCHEDULER_H

#include <QString>
#include "scheduledata.h"
#include "schedulererrormanager.h"

class AbstractScheduler
{
public:
   virtual bool Read(ScheduleData** data) const = 0;
   virtual bool Write(ScheduleData* data) = 0;

   QString GetLastError() const;
   void SetCommandToRun(const QString& value);
   void SetCommandArguments(const QString& value);

protected:
   SchedulerErrorManager errorManager;
   QString commandToRun = "";
   QString commandArguments = "";
};

#endif // ABSTRACTSCHEDULER_H
