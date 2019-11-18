#include "windowsmessagecreator.h"

#include <Windows.h>
#include <shellapi.h>

WindowsMessageCreator::WindowsMessageCreator(const unsigned long _shellExecuteCode)
   : shellExecuteReturnCode(_shellExecuteCode)
{
}

QString WindowsMessageCreator::CreateMessage(
        const TaskToolConfigurationData& configurationData,
        const TaskToolRunData& runData
        )
{
    if (IsShellExecuteSuccess())
        return ErrorMessageCreator::CreateMessage(configurationData, runData);
    else
       return CreateShellExecuteError();
}

QString WindowsMessageCreator::CreateShellExecuteError() const
{
   QString errorMessage = "Error running Task tool command.\n";
   errorMessage += "\t" + CreateWindowsErrorMessage() + "\n";
   return errorMessage;
}

QString WindowsMessageCreator::CreateWindowsErrorMessage() const
{
   LPSTR messageBuffer = nullptr;
   const DWORD flags = FORMAT_MESSAGE_ALLOCATE_BUFFER |
                 FORMAT_MESSAGE_FROM_SYSTEM |
                 FORMAT_MESSAGE_IGNORE_INSERTS;
   const size_t size = FormatMessage(
                          flags, NULL,
                          shellExecuteReturnCode,
                          MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                          (LPSTR)&messageBuffer,
                          0, NULL);

   std::string message(messageBuffer, size);
   LocalFree(messageBuffer);
   return QString::fromStdString(message);
}

bool WindowsMessageCreator::IsShellExecuteSuccess() const
{
    return (shellExecuteReturnCode > 32);
}
