#include "taskfeaturetest.h"

#include <list>
#include <QDir>
#include <QTest>

#include "maintoolmodule.h"
#include "clientworkmanager.h"
#include "configuration.h"
#include "curlconsoleemailsender.h"
#include "filetestutils.h"
#include "filetools.h"
#include "gittools.h"
#include "tools.h"

using namespace std;

static const string suiteTag = "TaskFeature/";
static const string reportDir = "report/";
static const string reportFile = "report.html";

TaskFeatureTest::TaskFeatureTest(const std::string& _dataFolder,
                                 const std::string& _errorFolder)
    : suiteFolder(_dataFolder + suiteTag), errorFolder(_errorFolder)
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
    QFETCH(QString, configurationFile);

    MainToolModule mainTool("AutoTest");
    mainTool.EnableTimedRuns(false);

    map<string,string> parameters;
    parameters["conffile"] = currentTestCaseFolder + configurationFile.toStdString();
    mainTool.RunFromParameterMap(parameters);

    CheckFinalReport();
}

void TaskFeatureTest::CheckFinalReport()
{
    const string reportContents = FileTools::GetTextFileContent(reportDir + reportFile);
    vector<string> attachments = GetAttachmentFiles();

    CheckReport(reportContents);
    CheckAttachments(attachments);
}

vector<string> TaskFeatureTest::GetAttachmentFiles() const
{
    QStringList fileList = FileTestUtils::GetFileList(reportDir.c_str());
    fileList.removeAll(reportFile.c_str());

    vector<string> stdFileList;
    for (auto&& it : fileList)
        stdFileList.push_back(it.toStdString());
    return stdFileList;
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

    vector<string> errors;
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

void TaskFeatureTest::CheckAttachments(const std::vector<string> &files)
{   
    vector<string> resultAttachmentContents;
    for (auto&& it : files)
    {
        const string content = FileTools::GetTextFileContent(reportDir + it);
        resultAttachmentContents.push_back(content);
    }

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

void TaskFeatureTest::CheckAttachmentContentsAreEqual(const std::vector<string> &contents,
                                                      const std::vector<string> &expectedContents)
{
    if (contents.size() != expectedContents.size())
    {
        for (unsigned int i=0; i<contents.size(); ++i)
            WriteAttachment(contents.at(i), i+1);
        QFAIL("Wrong number of attachments");
    }

    unsigned int counter = 1;
    for (auto&& itExpected : expectedContents)
    {
        auto itElement = find(contents.begin(), contents.end(), itExpected);
        if (itElement == contents.end())
        {
            WriteAttachment(itExpected, counter);
            QFAIL("Attachment not found");
        }
        ++counter;
    }
}

void TaskFeatureTest::WriteAttachment(const string &content, const int number)
{
    stringstream name;
    name << "attachment" << number << ".txt";
    FileTools::WriteBufferToFile(errorFolder + currentTestCaseName + "_" + name.str(), content);
}
