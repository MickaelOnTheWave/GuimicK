#include "abstractscheduler.h"

void AbstractScheduler::SetCommandToRun(const QString& value)
{
   commandToRun = value;
}

void AbstractScheduler::SetCommandArguments(const QString& value)
{
   commandArguments = value;
}
