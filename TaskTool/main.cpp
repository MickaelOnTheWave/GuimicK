#include "curldispatcherreplacer.h"
#include "maintoolmodule.h"

static const std::string PROGRAM_VERSION = "0.719";

int main(int argc, char* argv[])
{
    MainToolModule mainTool(PROGRAM_VERSION);
    mainTool.SetDispatcherReplacer(new CurlDispatcherReplacer());
    return mainTool.RunFromCommandLine(argc, argv);
}
