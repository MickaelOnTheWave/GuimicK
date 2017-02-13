#ifndef TASKFEATURETEST_H
#define TASKFEATURETEST_H

#include <QObject>
#include <QStringList>

#include "configuration.h"

class TaskFeatureTest : public QObject
{
    Q_OBJECT

public:
    TaskFeatureTest();

private Q_SLOTS:
    void init();
    void cleanup();

    void testGitBackup_data();
    void testGitBackup();

private:
    void ReadConfiguration(Configuration& configuration);
    void GetRepositoriesFromConfiguration(const Configuration &configuration,
                                          std::vector<std::pair<std::string,std::string> >& repositories);
    void SetupRepositoriesForBackup(const std::vector<std::pair<std::string,std::string> >& repositories);
    WorkResultData* RunJobList(const Configuration& configuration);
    void CheckResults(WorkResultData* results, const Configuration& configuration);
    void RestoreOriginalData(const std::vector<std::pair<std::string,std::string> >& repositories);

    void CreateCopyIfPossible(const std::string& folder);
    void CheckMainReport(const std::string& reportContent);
    void CheckReportAttachments(std::vector<std::string> &_externalFiles,
                                std::vector<std::pair<std::string,std::string> >& _fileBuffers);

    void CheckAttachments(const std::vector<std::pair<std::string,std::string> >& attachments);

    void RestoreCopyIfAvailable(const std::string &folder);

    std::vector<std::pair<std::string,std::string> > currentRepositories;
};

#endif // TASKFEATURETEST_H
