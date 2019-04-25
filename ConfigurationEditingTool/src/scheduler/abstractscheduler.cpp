#include "abstractscheduler.h"

QString AbstractScheduler::GetLastError() const
{
   return lastError;
}

void AbstractScheduler::SetCommandToRun(const QString& value)
{
   commandToRun = value;
}

void AbstractScheduler::SetCommandArguments(const QString& value)
{
   commandArguments = value;
}
