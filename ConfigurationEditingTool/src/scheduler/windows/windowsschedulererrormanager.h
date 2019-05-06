#ifndef SCHEDULERERRORMANAGER_H
#define SCHEDULERERRORMANAGER_H

#include <comdef.h>
#include "baseschedulererrormanager.h"

class WindowsSchedulerErrorManager : public BaseSchedulerErrorManager
{
public:
   WindowsSchedulerErrorManager() = default;
   virtual ~WindowsSchedulerErrorManager() = default;

   void UpdateLastErrorMessage(const HRESULT hr);
};

#endif // SCHEDULERERRORMANAGER_H
