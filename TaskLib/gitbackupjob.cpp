#include "gitbackupjob.h"

#include <sstream>
#include <filetools.h>
#include <unistd.h>

using namespace std;

GitBackupJob::GitBackupJob()
{
}

GitBackupJob::~GitBackupJob()
{
}

string GitBackupJob::CorrectRepositoryWord(int n)
{
    return (n == 1) ? "repository" : "repositories";
}
