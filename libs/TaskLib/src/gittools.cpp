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

vector<wstring> GitTools::GetRemotesUrls()
{
   ConsoleJob command = GitTools::Run(L"remote -v");
   return GitTools::GetRemotesUrlsFromCommandOutput(command.GetCommandOutput());
}

vector<wstring> GitTools::GetRemotesUrlsFromCommandOutput(const wstring& output)
{
   vector<wstring> rawRemotes;
   StringTools::Tokenize(output, L'\n', rawRemotes);
   return CreateRemotesUrls(rawRemotes);
}

vector<wstring> GitTools::CreateRemotesUrls(const vector<wstring>& remotesLines)
{
   vector<wstring> remotesUrls;
   vector<wstring>::const_iterator it=remotesLines.begin();
   for(; it!=remotesLines.end(); ++it)
      remotesUrls.push_back(GetRemoteUrl(*it));
   return remotesUrls;
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
