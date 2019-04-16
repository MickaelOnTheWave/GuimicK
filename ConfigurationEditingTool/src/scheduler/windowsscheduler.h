#ifndef WINDOWSSCHEDULER_H
#define WINDOWSSCHEDULER_H

#include "abstractscheduler.h"

class WindowsScheduler : public AbstractScheduler
{
public:
   WindowsScheduler();

   bool Read(ScheduleTarget &data) override;
   bool Write(const ScheduleTarget &data) override;
};

#endif // WINDOWSSCHEDULER_H
