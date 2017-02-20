#include "taskfeaturetest.h"

#include <list>
#include <QDir>
#include <QTest>

#include "clientworkmanager.h"
#include "configuration.h"
#include "curlconsoleemailsender.h"
#include "filetestutils.h"
#include "filetools.h"
#include "gittools.h"
#include "tools.h"

using namespace std;

const string suiteFolder = "../TaskFeature/";

TaskFeatureTest::TaskFeatureTest()
{
}

void TaskFeatureTest::init()
{
    currentTestCaseName = QTest::currentDataTag();
    currentTestCaseFolder = suiteFolder + currentTestCaseName + "/";

    CopyDataFolders();
}

void TaskFeatureTest::cleanup()
{
    string unusedOutput;
    for (auto it : currentDataFolders)
    {
        string command = string("rm -Rf ") + it.toStdString();
        Tools::RunExternalCommandToBuffer(command, unusedOutput, true);
    }
    currentDataFolders.clear();
}

void TaskFeatureTest::testRun_data()
{
    QTest::addColumn<QString>("configurationFile");
    QTest::addColumn<QString>("reportFile");
    QTest::addColumn<QStringList>("attachmentFiles");

    QStringList testCases = FileTestUtils::GetFolderList(suiteFolder.c_str());
    for (auto it=testCases.begin(); it!=testCases.end(); ++it)
    {
        if (*it == "." || *it == "..")
            continue;

        string stdString = it->toStdString();
        QStringList attachments = GetAttachmentFiles(suiteFolder + "/" + stdString);
        QTest::newRow(stdString.c_str())    << QString("configuration.txt")
                                            << QString("report.html")
                                            << attachments;
    }
}

void TaskFeatureTest::testRun()
{
    Configuration configuration;
    ReadConfiguration(configuration);

    ClientWorkManager * manager = configuration.BuildSimpleWorkList();
    WorkResultData* results = manager->RunWorkList();

    AbstractReportCreator* reportCreator = configuration.CreateReportObject();
    string reportContents = reportCreator->Generate(results, "AutoTest");
    CheckReport(reportContents);

    CheckAttachments(results);

    delete manager;
    delete results;
}

void TaskFeatureTest::CopyDataFolders()
{
    string unusedOutput;
    currentDataFolders = FileTestUtils::GetFolderList(currentTestCaseFolder.c_str());
    for (auto it : currentDataFolders)
    {
        string currentFolder = it.toStdString();
        string command = string("cp -R ") + currentTestCaseFolder + currentFolder + " ./";
        Tools::RunExternalCommandToBuffer(command, unusedOutput, true);
    }
}

QStringList TaskFeatureTest::GetAttachmentFiles(const string &folder)
{
    QDir currentDir = QDir::current();
    currentDir.cd(folder.c_str());
    return currentDir.entryList(QStringList({"attachment*.txt"}), QDir::Files);
}

void TaskFeatureTest::ReadConfiguration(Configuration &configuration)
{
    QFETCH(QString, configurationFile);

    list<string> errors;
    const string configurationFilePath = currentTestCaseFolder + configurationFile.toStdString();
    bool confOk = configuration.LoadFromFile(configurationFilePath, errors);
    QCOMPARE(confOk, true);
    QCOMPARE(errors.size(), 0ul);
}

void TaskFeatureTest::CheckReport(const string &reportContent)
{
    QFETCH(QString, reportFile);
    const string referenceReportFile = currentTestCaseFolder + reportFile.toStdString();
    string expectedContent = FileTools::GetTextFileContent(referenceReportFile);
    bool areContentAsExpected = (reportContent == expectedContent);
    if (areContentAsExpected == false)
        FileTools::WriteBufferToFile(currentTestCaseName + "_" + reportFile.toStdString(),
                                     reportContent);
    QCOMPARE(reportContent, expectedContent);
}

void TaskFeatureTest::CheckAttachments(WorkResultData *results)
{
    QVERIFY(results->allClientsResults.size() == 1);

    ClientJobResults* allResults = results->allClientsResults.front().second;
    QVERIFY(allResults != nullptr);

/*    vector<pair<string, string> > allAttachments;
    allResults->GetAllAttachments(allAttachments);

    QFETCH(QStringList, attachmentFiles);

    QCOMPARE(allAttachments.size(), attachmentFiles.size());*/
    QFAIL("Finish implementation - comparing attachments.");
}
