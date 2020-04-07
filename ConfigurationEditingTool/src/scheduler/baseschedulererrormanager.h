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

   void SetLastError(const QString& message)
   {
       lastError = message;
   }

protected:
   QString lastError = "";
};

#endif // BASECHEDULERERRORMANAGER_H
