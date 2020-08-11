#ifndef GITTOOLS_H
#define GITTOOLS_H

#include "abstractbackupjob.h"
#include "consolejob.h"

class GitTools
{
public:
    static bool ChangeCurrentDir(const std::wstring &newDir,
                                 AbstractBackupJob::ResultCollection &statusList);

    static JobStatus* ChangeCurrentDir(const std::wstring &newDir);

    static ConsoleJob Run(const std::wstring& parameters);

    static ConsoleJob CreateRunJob(const std::wstring& parameters);

    static std::wstring GetRemoteUrlFromCommandOutput(const std::wstring& output);

private:
    static std::wstring GetRemoteUrl(const std::wstring& lineOutput);
};

#endif // GITCOMMONTOOLS_H
