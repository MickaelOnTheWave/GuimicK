#include "curldispatcherreplacer.h"
#include "maintoolmodule.h"
#include "ostools.h"
#include "stringtools.h"

#ifdef _WIN32
    #include <Windows.h>
    #include <processenv.h>
    #include <shellapi.h>
#endif

using namespace std;

wstring GetVersionTag()
{
#ifdef VERSION
   return StringTools::Utf8ToUnicode(VERSION);
#else
   return L"1.0.20-static";
#endif
}

int RunMain(int argc, char *argv[])
{
   MainToolModule mainTool(GetVersionTag());
   mainTool.SetDispatcherReplacer(new CurlDispatcherReplacer());

#ifdef WIN32
   int argCount;
   LPWSTR* args = CommandLineToArgvW(GetCommandLineW(), &argCount);
   return mainTool.RunFromCommandLine(argCount, args);
#else
   return mainTool.RunFromCommandLine(argc, argv);
#endif
}

bool NeedsAdminElevation()
{
   return OsTools::IsOnWindows() && !OsTools::IsRunningAsAdministrator();
}

string CreateParameters(int argc, char* argv[])
{
   string serializedParameters;
   for (int i=1; i<argc; ++i)
   {
      serializedParameters += argv[i];
      serializedParameters += " ";
   }
   return serializedParameters;
}

int RunElevated(int argc, char* argv[])
{
#ifdef _WIN32
   const string parameters = CreateParameters(argc, argv);
   char szPath[MAX_PATH];
   if (GetModuleFileName(NULL, szPath, ARRAYSIZE(szPath)))
   {
      SHELLEXECUTEINFO sei = { sizeof(sei) };

      sei.lpVerb = "runas";
      sei.lpFile = szPath;
      sei.lpParameters = parameters.c_str();
      sei.hwnd = NULL;
      sei.nShow = SW_NORMAL;

      ShellExecuteEx(&sei);
   }
#endif
   return 0;
}

int main(int argc, char* argv[])
{
   if (NeedsAdminElevation())
      return RunElevated(argc, argv);
   else
      return RunMain(argc, argv);
}
