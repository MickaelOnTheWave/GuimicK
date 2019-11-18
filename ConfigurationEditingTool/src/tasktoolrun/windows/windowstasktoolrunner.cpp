#include "windows/windowstasktoolrunner.h"

#include <Windows.h>
#include <shellapi.h>

#include "windowsmessagecreator.h"

void WindowsTaskToolRunner::Run()
{
   const bool ok = ExecuteTaskToolAsAdmin();
   if (ok)
      WaitUntilExecutionIsComplete();
   else
      SetErrorMessage();

   emit finished();
}

ErrorMessageCreator* WindowsTaskToolRunner::CreateMessageCreator()
{
   return new WindowsMessageCreator(shellExecuteErrorCode);
}

bool WindowsTaskToolRunner::ExecuteTaskToolAsAdmin()
{
   HINSTANCE errorCode = ShellExecute(
            NULL,
            "runas",
            "c:\\Windows\\notepad.exe",
            "",
            "",
            SW_SHOWNORMAL
            );
   shellExecuteErrorCode = reinterpret_cast<unsigned long>(errorCode);
   return (shellExecuteErrorCode > 32 );
}

void WindowsTaskToolRunner::WaitUntilExecutionIsComplete()
{
   // TODO test and implement

}

void WindowsTaskToolRunner::SetErrorMessage()
{
   LPSTR messageBuffer = nullptr;
   const DWORD flags = FORMAT_MESSAGE_ALLOCATE_BUFFER |
                 FORMAT_MESSAGE_FROM_SYSTEM |
                 FORMAT_MESSAGE_IGNORE_INSERTS;
   const size_t size = FormatMessage(
                          flags, NULL,
                          shellExecuteErrorCode,
                          MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                          (LPSTR)&messageBuffer,
                          0, NULL);

   std::string message(messageBuffer, size);
   LocalFree(messageBuffer);
}
