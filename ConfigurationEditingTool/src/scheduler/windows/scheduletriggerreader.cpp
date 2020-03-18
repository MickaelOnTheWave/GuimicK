#include "scheduletriggerreader.h"

#include <comdef.h>
#include <math.h>

namespace
{
   QTime ConvertToQtime(const BSTR& time)
   {
      int year, month, day, hour, minute, second;
      swscanf_s(time, L"%4d-%02d-%02dT%02d:%02d:%02d",
                &year, &month, &day,
                &hour, &minute, &second);
      return QTime(hour, minute);
   }

   void SetCommonDataFromTrigger(ScheduleData* data, ITrigger* trigger)
   {
      BSTR triggerStart;
      HRESULT hr = trigger->get_StartBoundary(&triggerStart);
      if (SUCCEEDED(hr))
         data->SetTime(ConvertToQtime(triggerStart));
   }

   IDailyTrigger* GetDailyTrigger(ITrigger* trigger)
   {
      IDailyTrigger *dailyTrigger = nullptr;
      trigger->QueryInterface(IID_IDailyTrigger, (void**)&dailyTrigger);
      return dailyTrigger;
   }

   IWeeklyTrigger* GetWeeklyTrigger(ITrigger* trigger)
   {
      IWeeklyTrigger *weeklyTrigger = nullptr;
      trigger->QueryInterface(IID_IWeeklyTrigger, (void**)&weeklyTrigger);
      return weeklyTrigger;
   }

   IMonthlyTrigger* GetMonthlyTrigger(ITrigger* trigger)
   {
      IMonthlyTrigger *monthlyTrigger = nullptr;
      trigger->QueryInterface(IID_IMonthlyTrigger, (void**)&monthlyTrigger);
      return monthlyTrigger;
   }

   void SetDayDataFromMask(ScheduleWeeklyData* data, const long dayMask, const long daysCount)
   {
      for (int i = 0; i < daysCount; ++i)
      {
         const long currentDayMask = pow(2, i);
         const long result = dayMask & currentDayMask;
         if (result == currentDayMask)
            data->AddDayIndex(i);
      }
   }

   ScheduleData* CreateDataFromDailyTrigger(ITrigger* trigger)
   {
      auto data = new ScheduleDailyData();
      SetCommonDataFromTrigger(data, trigger);
      return data;
   }

   ScheduleData* CreateDataFromWeeklyTrigger(ITrigger* trigger)
   {
      auto data = new ScheduleWeeklyData();
      SetCommonDataFromTrigger(data, trigger);
      IWeeklyTrigger* weeklyTrigger = GetWeeklyTrigger(trigger);
      if (weeklyTrigger)
      {
         short dayMask;
         weeklyTrigger->get_DaysOfWeek(&dayMask);
         SetDayDataFromMask(data, dayMask, 7);
      }
      return data;
   }

   ScheduleData* CreateDataFromMonthlyTrigger(ITrigger* trigger)
   {
      auto data = new ScheduleMonthlyData();
      SetCommonDataFromTrigger(data, trigger);
      IMonthlyTrigger* monthlyTrigger = GetMonthlyTrigger(trigger);
      if (monthlyTrigger)
      {
         long dayMask;
         monthlyTrigger->get_DaysOfMonth(&dayMask);
         SetDayDataFromMask(data, dayMask, 31);
      }
      return data;
   }
}

ScheduleData* ScheduleTriggerReader::Read(ITrigger* trigger)
{
   TASK_TRIGGER_TYPE2 triggerType;
   HRESULT hr = trigger->get_Type(&triggerType);
   if (SUCCEEDED(hr))
   {
      if (triggerType == TASK_TRIGGER_DAILY)
         return CreateDataFromDailyTrigger(trigger);
      else if (triggerType == TASK_TRIGGER_WEEKLY)
         return CreateDataFromWeeklyTrigger(trigger);
      else if (triggerType == TASK_TRIGGER_MONTHLY)
         return CreateDataFromMonthlyTrigger(trigger);
   }

   return nullptr;
}
