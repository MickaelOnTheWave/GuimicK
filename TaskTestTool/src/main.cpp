#include <iostream>
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
const string progDir = "/home/mickael/Prog";
const string taskManagerDir = progDir + "/TaskManager";
const string taskLibDir  = taskManagerDir + "/TaskTestLib/data/";
const string parsersDir  = taskManagerDir + "/ReportParsers/ParsersTestLib/data/";
const string toolsLibDir = progDir + "/Tools/ToolsTestLib/data/";
const string errorDir    = taskManagerDir + "/TaskTestLib/data/errors/";

const string testingFolder = "TaskManagerTestSuites/";

int main(int argc, char* argv[])
{
    if (FileTools::FolderExists(testingFolder) == false)
       FileTools::CreateFolder(testingFolder);

    const int ALL_OK = 0;
    const int ERRORS = 1;
    const int DID_NOT_RUN = 2;

    int result = chdir(testingFolder.c_str());
    if (result != 0)
    {
       cout << "Can't chdir to folder " << testingFolder << endl;
       return DID_NOT_RUN;
    }

    CommandLineManager commandLine(argc, argv);
    commandLine.AddParameter("silent", "Runs in silent mode, no output on console.");
    commandLine.AddParameter("detailed", "Displays detailed results.");
    commandLine.EnableHelpCommand();

    if (commandLine.HandleUnknownParameters())
        return DID_NOT_RUN;
    else if (commandLine.HandleHelpCommand())
        return DID_NOT_RUN;

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

