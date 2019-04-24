#ifndef SCHEDULEDATA_H
#define SCHEDULEDATA_H

#include <QTime>
#include <vector>

class ScheduleData
{
public:
   QTime GetTime() const;
   void SetTime(const QTime& value);

protected:
   QTime time;
};

class ScheduleDailyData : public ScheduleData
{
};

class ScheduleWeeklyData : public ScheduleData
{
public:
    void AddDayIndex(const int value);

protected:
    std::vector<int> daysIndices;
};

class ScheduleMonthlyData : public ScheduleWeeklyData
{
};

#endif // SCHEDULEDATA_H
