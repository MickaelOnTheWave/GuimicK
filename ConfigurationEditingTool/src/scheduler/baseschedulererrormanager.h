#ifndef BASECHEDULERERRORMANAGER_H
#define BASECHEDULERERRORMANAGER_H

#include <qstring.h>

class BaseSchedulerErrorManager
{
public:
   QString GetLastError() const
   {
       return lastError;
   }

protected:
   QString lastError = "";
};

#endif // BASECHEDULERERRORMANAGER_H
