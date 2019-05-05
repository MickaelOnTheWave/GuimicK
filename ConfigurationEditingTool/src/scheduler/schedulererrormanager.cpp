#include "schedulererrormanager.h"

void SchedulerErrorManager::UpdateLastErrorMessage(const HRESULT hr)
{
   _com_error comError(hr);
   const LPCTSTR errorMsg = comError.ErrorMessage();
   lastError = errorMsg;
}

QString SchedulerErrorManager::GetLastError() const
{
   return lastError;
}