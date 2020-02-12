#include "curldispatcherreplacer.h"
#include "maintoolmodule.h"

#ifdef WIN32
    #include <Windows.h>
    #include <processenv.h>
    #include <shellapi.h>
#endif

// TODO : put version number on Cmake for coherence with
// editor, tool and installer
static const std::wstring PROGRAM_VERSION = L"1.0.18";

int main(int argc, char* argv[])
{
    MainToolModule mainTool(PROGRAM_VERSION);
    mainTool.SetDispatcherReplacer(new CurlDispatcherReplacer());
    
#ifdef WIN32
    int argCount;
    LPWSTR* args = CommandLineToArgvW(GetCommandLineW(), &argCount);
    return mainTool.RunFromCommandLine(argCount, args);
#else
    return mainTool.RunFromCommandLine(argc, argv);
#endif
}
