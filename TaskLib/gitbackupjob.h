#ifndef GITBACKUPJOB_H
#define GITBACKUPJOB_H

#include "abstractjob.h"

/**
 * @brief Base class for git based backups
 */
class GitBackupJob : public AbstractJob
{
public:
    GitBackupJob();
    virtual ~GitBackupJob();

protected:
    std::string CorrectRepositoryWord(int n);
};

#endif // GITBACKUPJOB_H
