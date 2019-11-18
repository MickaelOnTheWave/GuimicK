#ifndef WINDOWSMESSAGECREATOR_H
#define WINDOWSMESSAGECREATOR_H

#include "errormessagecreator.h"

class WindowsMessageCreator : public ErrorMessageCreator
{
public:
    WindowsMessageCreator(const unsigned long _shellExecuteCode);
    virtual ~WindowsMessageCreator() = default;

   virtual QString CreateMessage(
       const TaskToolConfigurationData& configurationData,
       const TaskToolRunData& runData
   );

private:
    QString CreateShellExecuteError() const;
    QString CreateWindowsErrorMessage() const;


    bool IsShellExecuteSuccess() const;

    unsigned long shellExecuteReturnCode = 0;
};

#endif // WINDOWSMESSAGECREATOR_H
