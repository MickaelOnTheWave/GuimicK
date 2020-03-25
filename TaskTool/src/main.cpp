#include "curldispatcherreplacer.h"
#include "maintoolmodule.h"

#ifdef WIN32
    #include <Windows.h>
    #include <processenv.h>
    #include <shellapi.h>
#endif

std::wstring GetVersionTag()
{
#ifdef VERSION
   return VERSION;
#else
   return L"1.0.20-static";
#endif
}

int main(int argc, char* argv[])
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
