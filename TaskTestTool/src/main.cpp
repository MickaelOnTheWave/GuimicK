#include <iostream>
#include <unistd.h>

#include "commandlinemanager.h"
#include "filetools.h"
#include "pathtools.h"
#include "qtbatchtestrunner.h"
#include "stringtools.h"

#include "parserstestlib.h"
#include "qttoolstestlib.h"
#include "tasktestlib.h"
#include "toolstestlib.h"

using namespace std;

// @TODO : Make these folders relative and update Cmake to have an install target
// that copies test data to binary dir.
const wstring progDir = L"/home/mickael/Prog";
const wstring taskManagerDir = progDir + L"/TaskManager";
const wstring taskLibDir  = taskManagerDir + L"/TaskTestLib/data/";
const wstring parsersDir  = taskManagerDir + L"/ReportParsers/ParsersTestLib/data/";
const wstring toolsLibDir = progDir + L"/Tools/ToolsTestLib/data/";
const wstring errorDir    = taskManagerDir + L"/TaskTestLib/data/errors/";

const wstring testingFolder = L"TaskManagerTestSuites/";

int main(int argc, char* argv[])
{
    if (FileTools::FolderExists(testingFolder) == false)
       FileTools::CreateFolder(testingFolder);

    const int ALL_OK = 0;
    const int ERRORS = 1;
    const int DID_NOT_RUN = 2;

    const bool ok = PathTools::ChangeCurrentDir(testingFolder);
    if (!ok)
    {
       wcout << "Can't chdir to folder " << testingFolder << endl;
       return DID_NOT_RUN;
    }

    CommandLineManager commandLine(argc, argv);
    commandLine.AddParameter(L"silent", L"Runs in silent mode, no output on console.");
    commandLine.AddParameter(L"detailed", L"Displays detailed results.");
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
    runner.SetTempResultFile(StringTools::UnicodeToUtf8(errorDir + L"result.xml"));
    runner.Run(tests);

    if (commandLine.HasParameter(L"silent") == false)
    {
        if (commandLine.HasParameter(L"detailed"))
            runner.ShowFullData();
        else
            runner.ShowSummarizedData();
    }

    FreeTests(tests);
    return (runner.HaveAllPassed() ? ALL_OK : ERRORS);
}

