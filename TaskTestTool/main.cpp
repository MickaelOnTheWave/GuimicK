#include "commandlinemanager.h"
#include "qtbatchtestrunner.h"
#include "tasktestlib.h"

using namespace std;

int main(int argc, char* argv[])
{
    const int ALL_OK = 0;
    const int ERRORS = 1;
    const int DID_NOT_RAN = 2;

    CommandLineManager commandLine(argc, argv);
    commandLine.AddParameter("silent", "Runs in silent mode, no output on console.");
    commandLine.AddParameter("detailed", "Displays detailed results.");
    commandLine.EnableHelpCommand();

    if (commandLine.HandleUnknownParameters())
        return DID_NOT_RAN;
    else if (commandLine.HandleHelpCommand())
        return DID_NOT_RAN;

    vector<QObject*> tests;
    GetAllTests(tests);

    QtBatchTestRunner runner(argv[0]);
    runner.SetTempResultFile("../errors/result.xml");
    runner.Run(tests);

    if (commandLine.HasParameter("silent") == false)
    {
        if (commandLine.HasParameter("detailed"))
            runner.ShowFullData();
        else
            runner.ShowSummarizedData();
    }

    FreeTests(tests);
    return (runner.HaveAllPassed() ? ALL_OK : ERRORS);
}

