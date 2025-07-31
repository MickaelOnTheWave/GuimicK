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
    void testCreateOne_Ok();

    void testUpdate_data();
    void testUpdate();
    void testUpdate_MultipleRepositories();
    void testUpdate_InvalidSource();

private:
    using RepositoryList = std::vector<std::pair<std::wstring, std::wstring>>;
    std::vector<GitRepository*> CreateMultipleRepositories();
    void CreateInitialRepositoryData(const std::vector<GitRepository*>& repositories);
    void CreateDefaultData(const std::wstring& repository);

    void RunGitBackup(const std::wstring &source, const std::wstring &destination);
    void RunGitBackup(const RepositoryList& repositoryList);

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
    const RepositoryList CreateRepositoryListForBackup(
            const std::vector<GitRepository*>& repositories
    );

    void CheckGitHeadContent(const std::wstring& repository,
                             const QStringList &expectedContents);

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
