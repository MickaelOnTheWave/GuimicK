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
const string errorFolder = "../errors/";

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
    Tools::RunExternalCommandToBuffer("rm -Rf *", unusedOutput, true);
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

    AbstractReportCreator* reportCreator = configuration.GetReportCreator();
    reportCreator->Generate(results, "AutoTest");
    CheckReport(reportCreator->GetReportContent());

    CheckAttachments(results);

    delete manager;
    delete results;
}

void TaskFeatureTest::CopyDataFolders()
{
    string unusedOutput;
    QStringList currentDataFolders = FileTestUtils::GetFolderList(currentTestCaseFolder.c_str());
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
        FileTools::WriteBufferToFile(errorFolder + currentTestCaseName + "_" + reportFile.toStdString(),
                                     reportContent);
    QCOMPARE(reportContent, expectedContent);
}

void TaskFeatureTest::CheckAttachments(WorkResultData *results)
{
    QVERIFY(results->GetClientCount() == 1);

    vector<string> resultAttachmentContents;
    results->GetAttachmentContents(resultAttachmentContents);

    QFETCH(QStringList, attachmentFiles);
    vector<string> expectedAttachmentContents;
    GetAttachmentContents(attachmentFiles, expectedAttachmentContents);

    CheckAttachmentContentsAreEqual(resultAttachmentContents, expectedAttachmentContents);
}

void TaskFeatureTest::GetAttachmentContents(const QStringList &fileList, std::vector<string> &contentList)
{
    for (auto it : fileList)
    {
        string content = FileTools::GetTextFileContent(currentTestCaseFolder + it.toStdString());
        contentList.push_back(content);
    }
}

void TaskFeatureTest::CheckAttachmentContentsAreEqual(const std::vector<string> &contents, const std::vector<string> &expectedContents)
{
    QVERIFY(contents.size() == expectedContents.size());

    auto it=contents.begin();
    auto itExp=expectedContents.begin();
    int counter = 1;
    while (it!=contents.end() && itExp!=expectedContents.end())
    {
        bool isAsExpected = (*it == *itExp);
        if (isAsExpected == false)
        {
            stringstream name;
            name << "attachment" << counter << ".txt";
            FileTools::WriteBufferToFile(errorFolder + currentTestCaseName + "_" + name.str(), *it);
        }
        QCOMPARE(*it, *itExp);

        ++it;
        ++itExp;
        ++counter;
    }
}
