#ifndef GITJOBTEST_H
#define GITJOBTEST_H

#include <QObject>

#include "gitrepository.h"
#include "jobstatus.h"

class GitJobTest : public QObject
{
    Q_OBJECT

public:
    GitJobTest();

private Q_SLOTS:
    void init();
    void cleanup();

    void testCreate_InvalidSource();
    void testCreate_AllValid();

    void testUpdate_data();
    void testUpdate();
    void testUpdate_MultipleRepositories();
    void testUpdate_InvalidSource();

private:
    std::vector<GitRepository*> CreateMultipleRepositories();
    void CreateInitialRepositoryData(const std::vector<GitRepository*>& repositories);
    void CreateDefaultData(const std::string& repository);

    void RunGitBackup(const std::string &source, const std::string &destination);
    void RunGitBackup(const std::vector<std::pair<std::string, std::string> >& repositoryList);

    void CheckGitJobReturnsError(const QString& description);
    void CheckGitJobReturnsOk(const QString& description);
    void CheckGitJobReturn(const int expectedStatus,
                           const unsigned long expectedReportFileCount,
                           const QString& description);
    QString BuildDescriptionString(const int added, const int modified, const int deleted);
    QString BuildMultiDescriptionString(const std::vector<GitRepository*>& repositories);

    void UpdateSourceRepository(const QStringList& added,
                                const QStringList& modified,
                                const QStringList& removed);
    QStringList CreatedExpectedDestinationRepositoryContent(const QStringList& added,
                                                            const QStringList& removed);
    const std::vector<std::pair<std::string, std::string> > CreateRepositoryListForBackup(
            const std::vector<GitRepository*>& repositories
    );

    const QString sourceRepository = "GitRepository";
    const QString destinationRepository = "GitDestRepository";
    const QString invalidRepository = "blablabla";
    const QString existingFolder = "existingFolder";

    const QString messageInvalidSource = "Invalid source repository";
    const QString messageRepositoryCreationOk = "Repository cloned successfully";
    const QString messageRepositoryUpdateOk = "Repository successfully updated";

    const QStringList defaultRepositoryContent = {"file0", "file1", "file2", "file3"};

    JobStatus* currentStatus = nullptr;
};

#endif // GITJOBTEST_H
