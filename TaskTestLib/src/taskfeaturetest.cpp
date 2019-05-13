#include "taskfeaturetest.h"

#include <list>
#include <QDir>
#include <QTest>

#include "maintoolmodule.h"
#include "clientworkmanager.h"
#include "curlconsoleemailsender.h"
#include "filereportdispatcher.h"
#include "filetestutils.h"
#include "filetools.h"
#include "gittesttools.h"
#include "serverconfiguration.h"
#include "stringtools.h"
#include "tools.h"

using namespace std;

namespace
{
   const wstring suiteTag = L"TaskFeature/";
   const wstring reportDir = L"report/";
   const wstring reportFile = L"report.html";
}

TaskFeatureTest::TaskFeatureTest(const wstring& _dataFolder,
                                 const wstring& _errorFolder)
    : suiteFolder(_dataFolder + suiteTag), errorFolder(_errorFolder)
{
}

void TaskFeatureTest::init()
{
    cleanup();
    currentTestCaseName = StringTools::Utf8ToUnicode(QTest::currentDataTag());
    currentTestCaseFolder = suiteFolder + currentTestCaseName + L"/";

    CopyDataFolders();
}

void TaskFeatureTest::cleanup()
{
    wstring unusedOutput;
    Tools::RunExternalCommandToBuffer(L"rm -Rf *", unusedOutput, true);
}

void TaskFeatureTest::testRun_data()
{
    QTest::addColumn<QString>("configurationFile");
    QTest::addColumn<QString>("reportFile");
    QTest::addColumn<QStringList>("attachmentFiles");

    QStringList testCases = FileTestUtils::GetFolderList(QString::fromStdWString(suiteFolder));
    for (const auto& testCase : testCases)
    {
        if (testCase == "." || testCase == "..")
            continue;

        const wstring stdString = testCase.toStdWString();
        QStringList attachments = GetAttachmentFiles(suiteFolder + L"/" + stdString);
        QTest::newRow(testCase.toUtf8())    << QString("configuration.txt")
                                            << QString("report.html")
                                            << attachments;
    }
}

void TaskFeatureTest::testRun()
{
    QFETCH(QString, configurationFile);

    MainToolModule mainTool(L"AutoTest");
    mainTool.EnableTimedRuns(false);
    mainTool.SetFallbackDispatcher(new FileReportDispatcher());

    map<wstring,wstring> parameters;
    parameters[L"conffile"] = currentTestCaseFolder + configurationFile.toStdWString();
    mainTool.RunFromParameterMap(parameters);

    CheckFinalReport();
}

void TaskFeatureTest::CheckFinalReport()
{
    const wstring reportContents = FileTools::GetTextFileContent(reportDir + reportFile);
    const vector<wstring> attachments = GetAttachmentFiles();

    CheckReport(reportContents);
    CheckAttachments(attachments);
}

TaskFeatureTest::StdStringVector TaskFeatureTest::GetAttachmentFiles() const
{
    QStringList fileList = FileTestUtils::GetFileList(QString::fromStdWString(reportDir));
    fileList.removeAll(QString::fromStdWString(reportFile));

    StdStringVector stdFileList;
    for (auto&& it : fileList)
        stdFileList.push_back(it.toStdWString());
    return stdFileList;
}

void TaskFeatureTest::CopyDataFolders()
{
    wstring unusedOutput;
    QStringList currentDataFolders = FileTestUtils::GetFolderList(
             QString::fromStdWString(currentTestCaseFolder)
             );
    for (auto it : currentDataFolders)
    {
        const wstring currentFolder = it.toStdWString();
        const wstring command = L"cp -R " + currentTestCaseFolder + currentFolder + L" ./";
        Tools::RunExternalCommandToBuffer(command, unusedOutput, true);
    }
}

QStringList TaskFeatureTest::GetAttachmentFiles(const wstring &folder)
{
    QDir currentDir = QDir::current();
    currentDir.cd(QString::fromStdWString(folder));
    return currentDir.entryList(QStringList({"attachment*.txt"}), QDir::Files);
}

void TaskFeatureTest::ReadConfiguration(TaskManagerConfiguration& configuration)
{
    QFETCH(QString, configurationFile);

    StdStringVector errors;
    const wstring configurationFilePath = currentTestCaseFolder + configurationFile.toStdWString();
    bool confOk = configuration.LoadFromFile(configurationFilePath, errors);
    QCOMPARE(confOk, true);
    QCOMPARE(errors.size(), 0ul);
}

void TaskFeatureTest::CheckReport(const wstring& reportContent)
{
    QFETCH(QString, reportFile);
    const wstring stdReportFile = reportFile.toStdWString();

    const wstring referenceReportFile = currentTestCaseFolder + stdReportFile;
    const wstring expectedContent = FileTools::GetTextFileContent(referenceReportFile);
    bool areContentAsExpected = (reportContent == expectedContent);
    if (areContentAsExpected == false)
    {
        FileTools::WriteBufferToFile(
                 errorFolder + currentTestCaseName + L"_" + stdReportFile,
                 reportContent);
    }
    QCOMPARE(reportContent, expectedContent);
}

void TaskFeatureTest::CheckAttachments(const StdStringVector& files)
{   
    StdStringVector resultAttachmentContents;
    for (auto&& it : files)
    {
        const wstring content = FileTools::GetTextFileContent(reportDir + it);
        resultAttachmentContents.push_back(content);
    }

    QFETCH(QStringList, attachmentFiles);
    StdStringVector expectedAttachmentContents;
    GetAttachmentContents(attachmentFiles, expectedAttachmentContents);

    CheckAttachmentContentsAreEqual(resultAttachmentContents, expectedAttachmentContents);
}

void TaskFeatureTest::GetAttachmentContents(const QStringList &fileList,
                                            StdStringVector& contentList)
{
    for (auto&& it : fileList)
    {
        const wstring content = FileTools::GetTextFileContent(currentTestCaseFolder + it.toStdWString());
        contentList.push_back(content);
    }
}

void TaskFeatureTest::CheckAttachmentContentsAreEqual(const StdStringVector& contents,
                                                      const StdStringVector& expectedContents)
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

void TaskFeatureTest::WriteAttachment(const wstring &content, const int number)
{
    wstringstream name;
    name << "attachment" << number << ".txt";
    FileTools::WriteBufferToFile(errorFolder + currentTestCaseName + L"_" + name.str(), content);
}
