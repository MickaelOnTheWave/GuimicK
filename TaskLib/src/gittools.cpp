#include "gittools.h"

#include <string>
#include "pathtools.h"
#include "stringtools.h"

using namespace std;

static const wstring changeDirError                      = L"Failed accessing folder";

bool GitTools::ChangeCurrentDir(const wstring &newDir,
                                    AbstractBackupJob::ResultCollection &statusList)
{
   JobStatus* status = ChangeCurrentDir(newDir);
   if (!status->IsOk())
      statusList.push_back(BackupJobStatus(*status, NULL));
   return status->IsOk();
}

JobStatus* GitTools::ChangeCurrentDir(const wstring &newDir)
{
   const bool ok = PathTools::ChangeCurrentDir(newDir);
   return (ok) ? new JobStatus(JobStatus::Ok) : new JobStatus(JobStatus::Error, changeDirError);
}

ConsoleJob GitTools::Run(const wstring& parameters)
{
   ConsoleJob job = CreateRunJob(parameters);
   job.RunWithoutStatus();
   return job;
}

ConsoleJob GitTools::CreateRunJob(const wstring& parameters)
{
   const wstring gitAbsoluteCommand = PathTools::GetCommandPath(L"git", ConsoleJob::appSearchPaths);
   ConsoleJob job(gitAbsoluteCommand, parameters);
   return job;
}

wstring GitTools::GetRemoteUrlFromCommandOutput(const wstring& output)
{
   vector<wstring> remotes;
   StringTools::Tokenize(output, L'\n', remotes);
   return (remotes.empty()) ?  L"" : GetRemoteUrl(remotes.front());
}

std::wstring GitTools::GetRemoteUrl(const wstring& lineOutput)
{
   vector<wstring> blocks;
   StringTools::Tokenize(lineOutput, L' ', blocks);
   if (blocks.size() >= 3)
      return blocks[1];
   else if (!blocks.empty())
      return blocks.front();
   else
      return L"";
}
