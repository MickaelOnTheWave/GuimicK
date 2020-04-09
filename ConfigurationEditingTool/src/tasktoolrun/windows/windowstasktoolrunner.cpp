#include "windows/windowstasktoolrunner.h"

#include <Windows.h>
#include <shellapi.h>

#include "filetools.h"
#include "pathtools.h"
#include "stringtools.h"
#include "windowsmessagecreator.h"

WindowsTaskToolRunner::WindowsTaskToolRunner()
{
   CreateTaskToolLauncherFullPath();
}

ErrorMessageCreator* WindowsTaskToolRunner::CreateMessageCreator()
{
   return new WindowsMessageCreator(shellExecuteErrorCode);
}

void WindowsTaskToolRunner::RunFunction()
{
   runData.isOk = ExecuteTaskToolAsAdmin();
   if (runData.isOk)
      WaitUntilExecutionIsComplete();
   emit finished();
}

bool WindowsTaskToolRunner::ExecuteTaskToolAsAdmin()
{
   PrepareTaskToolLaunch();
   HINSTANCE errorCode = ShellExecute(
            NULL,
            "runas",
            launcherFullPath.c_str(),
            "",
            "",
            SW_HIDE
            );
   shellExecuteErrorCode = reinterpret_cast<unsigned long>(errorCode);
   return (shellExecuteErrorCode > 32 );
}

void WindowsTaskToolRunner::WaitUntilExecutionIsComplete()
{
   while (FileTools::FileExists(returnCodeFile) == false)
      Sleep(500);

   runData.returnCode = GetTaskToolReturnCode();
   runData.output = GetTaskToolOutput();
   runData.isOk = (runData.returnCode == 0);
}

void WindowsTaskToolRunner::PrepareTaskToolLaunch()
{
   FileTools::RemoveFile(returnCodeFile);

   const std::wstring envelopeInputFile = L"taskToolCommand";
   FileTools::WriteBufferToFile(envelopeInputFile, command);
}

void WindowsTaskToolRunner::CreateTaskToolLauncherFullPath()
{
   const std::wstring wstringLauncher = GetLauncherExecutablePath();
   launcherFullPath = StringTools::UnicodeToUtf8(wstringLauncher);
}

std::wstring WindowsTaskToolRunner::GetLauncherExecutablePath()
{
   const std::wstring launcherFile = L"\\TaskToolLauncher.exe";
   const std::wstring executionPath = PathTools::GetPathOnly(PathTools::GetCurrentExecutablePath());
   return executionPath + L"\\" + launcherFile;
}

int WindowsTaskToolRunner::GetTaskToolReturnCode() const
{
   return std::stoi(FileTools::GetTextFileContent(returnCodeFile));
}

std::wstring WindowsTaskToolRunner::GetTaskToolOutput() const
{
   return FileTools::GetTextFileContent(L"taskToolOutput");
}
