#ifndef SCHEDULERERRORMANAGER_H
#define SCHEDULERERRORMANAGER_H

#include <comdef.h>
#include <qstring.h>

class WindowsSchedulerErrorManager
{
public:
   WindowsSchedulerErrorManager() = default;
   ~WindowsSchedulerErrorManager() = default;

   void UpdateLastErrorMessage(const HRESULT hr);
   QString GetLastError() const;

private:
   QString lastError = "";
};

#endif // SCHEDULERERRORMANAGER_H
