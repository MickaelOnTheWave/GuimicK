#include "scheduletriggerwriter.h"

#include <comdef.h>

namespace
{
   wchar_t* defaultTriggerName = L"Default Trigger";

   BSTR ConvertToTimeString(const QTime& time)
   {
      SYSTEMTIME currentTime;
      GetLocalTime(&currentTime);

      const size_t bufferSize = 100;
      wchar_t buffer[bufferSize];
      swprintf_s(buffer, bufferSize, L"%4d-%02d-%02dT%02d:%02d:%02d", 
                 currentTime.wYear, currentTime.wMonth, currentTime.wDay,
                 time.hour(), time.minute(), time.second());
      return _bstr_t(buffer);
   }

   short ConvertToDayMask(const std::vector<int>& daysIndices)
   {
      short mask = 0;
      for (const int i : daysIndices)
         mask += pow(2, i);
      return mask;
   }

   short GetAllMonthsOfYearMask()
   {
      return pow(2, 12) - 1;
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

   bool SetDailyTriggerData(ITrigger* trigger, ScheduleDailyData*)
   {
      IDailyTrigger *dailyTrigger = GetDailyTrigger(trigger);
      bool ok = false;
      if (dailyTrigger)
      {
         const HRESULT hr = dailyTrigger->put_DaysInterval((short)1);
         dailyTrigger->Release();
         ok = SUCCEEDED(hr);
      }
      return ok;
   }

   bool SetWeeklyTriggerData(ITrigger* trigger, ScheduleWeeklyData* data)
   {
      bool ok = false;
      IWeeklyTrigger *weeklyTrigger = GetWeeklyTrigger(trigger);
      if (weeklyTrigger)
      {
         HRESULT hr = weeklyTrigger->put_WeeksInterval((short)1);
         if (SUCCEEDED(hr))
            hr = weeklyTrigger->put_DaysOfWeek(ConvertToDayMask(data->GetDaysIndices()));

         weeklyTrigger->Release();
         ok = SUCCEEDED(hr);
      }
      return ok;
   }

   bool SetMontlyTriggerData(ITrigger* trigger, ScheduleMonthlyData* data)
   {
      IMonthlyTrigger *monthlyTrigger = nullptr;
      HRESULT hr = trigger->QueryInterface(IID_IMonthlyTrigger, (void**)&monthlyTrigger);
      if (SUCCEEDED(hr))
      {
         hr = monthlyTrigger->put_DaysOfMonth(ConvertToDayMask(data->GetDaysIndices()));
         if (SUCCEEDED(hr))
            hr = monthlyTrigger->put_MonthsOfYear(GetAllMonthsOfYearMask());

         monthlyTrigger->Release();
      }
      return SUCCEEDED(hr);
   }

   bool SetTypedTriggerData(ITrigger* trigger, ScheduleData* data)
   {
      if (auto monthlyData = dynamic_cast<ScheduleMonthlyData*>(data))
         return SetMontlyTriggerData(trigger, monthlyData);
      else if (auto dailyData = dynamic_cast<ScheduleDailyData*>(data))
         return SetDailyTriggerData(trigger, dailyData);
      else if (auto weeklyData = dynamic_cast<ScheduleWeeklyData*>(data))
         return SetWeeklyTriggerData(trigger, weeklyData);
      else
         return true;
   }
}

ScheduleTriggerWriter::ScheduleTriggerWriter(SchedulerErrorManager& _errorManager)
   : errorManager(_errorManager)
{
}

bool ScheduleTriggerWriter::Write(ITrigger* trigger, ScheduleData* data)
{
   bool ok = SetCommonTriggerData(trigger, data);
   if (ok)
      ok = SetTypedTriggerData(trigger, data);
   return ok;
}

bool ScheduleTriggerWriter::SetCommonTriggerData(ITrigger* trigger, ScheduleData* data)
{
   HRESULT hr = trigger->put_Id(defaultTriggerName);
   if (SUCCEEDED(hr))
      hr = trigger->put_StartBoundary(ConvertToTimeString(data->GetTime()));

   errorManager.UpdateLastErrorMessage(hr);
   return SUCCEEDED(hr);
}
