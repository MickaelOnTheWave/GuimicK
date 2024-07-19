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

    static std::vector<std::wstring> GetRemotesUrls();

private:
    static std::vector<std::wstring> GetRemotesUrlsFromCommandOutput(
          const std::wstring& output
          );

    static std::vector<std::wstring> CreateRemotesUrls(
          const std::vector<std::wstring>& remotesLines
          );

    static std::wstring GetRemoteUrl(const std::wstring& lineOutput);
};

#endif // GITCOMMONTOOLS_H
