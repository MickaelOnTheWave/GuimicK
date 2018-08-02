#include <unistd.h>

#include "commandlinemanager.h"
#include "filetools.h"
#include "qtbatchtestrunner.h"

#include "parserstestlib.h"
#include "qttoolstestlib.h"
#include "tasktestlib.h"
#include "toolstestlib.h"

using namespace std;

// @TODO : Make these folders relative and update Cmake to have an install target
// that copies test data to binary dir.
const string taskLibDir  = "/home/mickael/Prog/TaskManager/TaskTestLib/data/";
const string parsersDir  = "/home/mickael/Prog/TaskManager/ReportParsers/ParsersTestLib/data/";
const string toolsLibDir = "/home/mickael/Prog/Tools/ToolsTestLib/data/";
const string errorDir    = "/home/mickael/Prog/TaskManager/TaskTestLib/data/errors/";

const string testingFolder = "TaskManagerTestSuites/";

int main(int argc, char* argv[])
{
    if (FileTools::FolderExists(testingFolder) == false)
       FileTools::CreateFolder(testingFolder);
    chdir(testingFolder.c_str());


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
    tests.reserve(100); // Some mysterious bug seems to happen if vector needs to be redimensioned

    GetTaskLibTests(tests, taskLibDir, errorDir);
    GetParserLibTests(tests, parsersDir);
    GetQtToolsLibTests(tests);
    GetToolsLibTests(tests, toolsLibDir);

    QtBatchTestRunner runner(argv[0]);
    runner.SetTempResultFile(errorDir + "result.xml");
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

