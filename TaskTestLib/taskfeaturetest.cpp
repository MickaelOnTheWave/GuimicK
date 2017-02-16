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

const string BACKUP_SUFFIX = "_CopyWhileTesting";

TaskFeatureTest::TaskFeatureTest()
{
}

void TaskFeatureTest::init()
{
    currentRepositories.clear();
}

void TaskFeatureTest::cleanup()
{
    RestoreOriginalData(currentRepositories);
}

void TaskFeatureTest::testGitBackup_data()
{
    QTest::addColumn<QString>("configurationFile");
    QTest::addColumn<QString>("outputReportFile");
    QTest::addColumn<QStringList>("outputAttachmentFiles");

    QTest::newRow("Single repository") << QString("ex1_1repository.txt")
                                       << QString("ex1_report.html")
                                       << QStringList({"ex1_dest.txt"});
//    QTest::newRow("2 repositories")    << QString("ex2_2repositories.txt")
//                                       << QString("ex2_report.html")
//                                       << QStringList({
//                                                          "ex2_attach1.txt",
//                                                          "ex2_attach2.txt"
//                                                      });
}

void TaskFeatureTest::testGitBackup()
{
    Configuration configuration;
    ReadConfiguration(configuration);

    GetRepositoriesFromConfiguration(configuration, currentRepositories);
    SetupRepositoriesForBackup(currentRepositories);

    WorkResultData* workResult = RunJobList(configuration);
    CheckResults(workResult, configuration);
}

void TaskFeatureTest::ReadConfiguration(Configuration &configuration)
{
    QFETCH(QString, configurationFile);

    list<string> errors;
    bool confOk = configuration.LoadFromFile(configurationFile.toStdString(), errors);
    QCOMPARE(confOk, true);
    QCOMPARE(errors.size(), 0ul);
}

void TaskFeatureTest::GetRepositoriesFromConfiguration(const Configuration &configuration,
                                                       vector<pair<string,string> >& repositories)
{
    list<AbstractJob*> jobList;
    ClientWorkManager* workList = configuration.BuildSimpleWorkList();
    workList->GetJobList(jobList);
    QCOMPARE(jobList.size(), 1ul);

    GitBackupJob* gitJob = dynamic_cast<GitBackupJob*>(jobList.front());
    QVERIFY(gitJob != nullptr);

    gitJob->GetRepositoryList(repositories);
    delete workList;
}

void TaskFeatureTest::SetupRepositoriesForBackup(const vector<pair<string,string> >& repositories)
{
    // In order for test to be cleaned and not depend on manual restoration,
    // we don't actually make the backup on repositories provided by configuration file.
    // Instead, we make a copy of both source and destination and run the backup on them.
    // This way, original data remains unchanged regardless of test runs :-)

    vector<pair<string,string> >::const_iterator it=repositories.begin();
    for (; it!=repositories.end(); ++it)
    {
        CreateCopyIfPossible(it->first);
        CreateCopyIfPossible(it->second);
    }
}

WorkResultData *TaskFeatureTest::RunJobList(const Configuration &configuration)
{
    ClientWorkManager* workList = configuration.BuildWorkList();
    WorkResultData* results =  workList->RunWorkList();
    //delete workList; // TODO : resolve leak!!
    return results;
}

void TaskFeatureTest::CheckResults(WorkResultData *results, const Configuration &configuration)
{
    const string version = "AutoTest";

    AbstractReportCreator* reportCreator = configuration.CreateReportObject();
    string reportData = reportCreator->Generate(results, version);
    CheckMainReport(reportData);

    vector<string> externalFiles;
    vector<pair<string,string> > fileBuffers;
    reportCreator->GetAssociatedFiles(externalFiles, fileBuffers);
    CheckReportAttachments(externalFiles, fileBuffers);
}

void TaskFeatureTest::RestoreOriginalData(const vector<pair<string, string> > &repositories)
{
    vector<pair<string,string> >::const_iterator it=repositories.begin();
    for (; it!=repositories.end(); ++it)
    {
        RestoreCopyIfAvailable(it->first);
        RestoreCopyIfAvailable(it->second);
    }
}

void TaskFeatureTest::CreateCopyIfPossible(const string &folder)
{
    QDir originalFolder(folder.c_str());
    if (originalFolder.exists() == false)
        return;

    string unusedOutput;
    string copyCommand = string("cp -R ") + folder + " " + folder + BACKUP_SUFFIX;
    Tools::RunExternalCommandToBuffer(copyCommand, unusedOutput, true);
}

void TaskFeatureTest::CheckMainReport(const string &reportContent)
{
    QFETCH(QString, outputReportFile);

    string expectedReportData = FileTools::GetTextFileContent(outputReportFile.toStdString());

    // Used to write data to create test.
    //FileTools::WriteBufferToFile("tempfile.txt", reportContent);

    QCOMPARE(reportContent, expectedReportData);
}

void TaskFeatureTest::CheckReportAttachments(vector<string> &_externalFiles,
                                             vector<pair<string, string> > &_fileBuffers)
{
    CheckAttachments(_fileBuffers);
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

        // Used to write data to create test.
        //string attachmentName = expectedAttachment + QString::number(i).toStdString() + ".txt";
        //FileTools::WriteBufferToFile(attachmentName, resultAttachment.second);

        QCOMPARE(resultAttachment.second, expectedContent);
    }
}

void TaskFeatureTest::RestoreCopyIfAvailable(const string &folder)
{
    string folderBackup = folder + BACKUP_SUFFIX;
    QDir dir(folderBackup.c_str());
    if (dir.exists() == false)
        return;

    FileTestUtils::RemoveAll(folder.c_str());

    string unusedOutput;
    string moveCommand = string("mv ") + folderBackup + " " + folder;
    Tools::RunExternalCommandToBuffer(moveCommand, unusedOutput, true);
}
