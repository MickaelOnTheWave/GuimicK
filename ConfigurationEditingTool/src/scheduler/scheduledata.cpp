#include "scheduledata.h"


QTime ScheduleData::GetTime() const
{
   return time;
}

void ScheduleData::SetTime(const QTime& value)
{
   time = value;
}

void ScheduleWeeklyData::AddDayIndex(const int value)
{
   daysIndices.push_back(value);
}

std::vector<int> ScheduleWeeklyData::GetDaysIndices() const
{
   return daysIndices;
}
