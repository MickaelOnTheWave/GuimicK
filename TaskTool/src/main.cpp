#include "curldispatcherreplacer.h"
#include "maintoolmodule.h"

static const std::wstring PROGRAM_VERSION = L"1.02";

int main(int argc, char* argv[])
{
    MainToolModule mainTool(PROGRAM_VERSION);
    mainTool.SetDispatcherReplacer(new CurlDispatcherReplacer());
    return mainTool.RunFromCommandLine(argc, argv);
}
