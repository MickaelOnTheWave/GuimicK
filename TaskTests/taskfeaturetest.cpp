#include "taskfeaturetest.h"

#include <list>
#include <QTest>

#include "clientworkmanager.h"
#include "configuration.h"
#include "curlconsoleemailsender.h"
#include "filetools.h"

using namespace std;

TaskFeatureTest::TaskFeatureTest()
{
}

void TaskFeatureTest::init()
{

}

void TaskFeatureTest::cleanup()
{

}

void TaskFeatureTest::testGitBackup_data()
{
    QTest::addColumn<QString>("configurationFile");
    QTest::addColumn<QString>("outputReportFile");
    QTest::addColumn<QStringList>("outputAttachmentFiles");

    QTest::newRow("Single repository") << QString("ex1_1repository.txt")
                                       << QString("ex1_report.txt")
                                       << QStringList({"ex1_attach1.txt"});
    QTest::newRow("2 repositories")    << QString("ex2_2repositories.txt")
                                       << QString("ex2_report.txt")
                                       << QStringList({
                                                          "ex2_attach1.txt",
                                                          "ex2_attach2.txt"
                                                      });
}

void TaskFeatureTest::testGitBackup()
{
    QFETCH(QString, configurationFile);
    QFETCH(QString, outputReportFile);
    const string version = "AutoTest";

    Configuration configuration;
    list<string> errors;

    bool confOk = configuration.LoadFromFile(configurationFile.toStdString(), errors);
    QCOMPARE(confOk, true);
    QCOMPARE(errors.size(), 0ul);

    ClientWorkManager* workList = configuration.BuildWorkList();
    WorkResultData* workResult = workList->RunWorkList();
    AbstractReportCreator* reportCreator = configuration.CreateReportObject();
    string reportData = reportCreator->Generate(workResult, version);
    string expectedReportData = FileTools::GetTextFileContent(outputReportFile.toStdString());
    QCOMPARE(reportData, expectedReportData);

    vector<string> externalFiles;
    vector<pair<string,string> > fileBuffers;
    reportCreator->GetAssociatedFiles(externalFiles, fileBuffers);

    CheckAttachments(fileBuffers);
}

void TaskFeatureTest::CheckAttachments(const vector<pair<string, string> >& attachments)
{
    QFETCH(QStringList, outputAttachmentFiles);

    QCOMPARE(attachments.size(), static_cast<unsigned long>(outputAttachmentFiles.size()));
    for (unsigned int i=0; i<attachments.size(); ++i)
    {
        pair<string,string> resultAttachment = attachments[i];
        string expectedAttachment = outputAttachmentFiles.at(i).toStdString();
        QCOMPARE(resultAttachment.first, expectedAttachment);

        string expectedContent = FileTools::GetTextFileContent(expectedAttachment);
        QCOMPARE(resultAttachment.second, expectedContent);
    }
}
