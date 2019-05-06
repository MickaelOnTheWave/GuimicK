#include "windowsschedulererrormanager.h"

void WindowsSchedulerErrorManager::UpdateLastErrorMessage(const HRESULT hr)
{
   _com_error comError(hr);
   const LPCTSTR errorMsg = comError.ErrorMessage();
   lastError = errorMsg;
}
