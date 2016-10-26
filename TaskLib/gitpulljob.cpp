#include "gitpulljob.h"

#include <sstream>
#include <filetools.h>
#include <unistd.h>

using namespace std;

GitPullJob::GitPullJob(std::vector<std::string> *_gitRepositoryList)
{
    if (_gitRepositoryList)
        gitRepositoryList = *_gitRepositoryList;
    else
        gitRepositoryList.clear();

    gitCommand = new ConsoleJob("", "git", "pull");
}

GitPullJob::~GitPullJob()
{
    delete gitCommand;
}

std::string GitPullJob::GetName()
{
    return "Git Backup (update)";
}

bool GitPullJob::InitializeFromClient(Client *)
{
    return true;
}


bool GitPullJob::IsInitialized()
{
    // 1. Check that git command is present
    if (!gitCommand->IsInitialized())
        return false;


    // 2. Check that all repositories are valid directories
    // Check that all repositories are valid directories
    vector<string>::const_iterator it=gitRepositoryList.begin();
    vector<string>::const_iterator end=gitRepositoryList.end();
    for (; it!=end; it++)
    {
        if (!FileTools::FolderExists(*it))
            return false;
    }
    return true;
}

JobStatus *GitPullJob::Run()
{
    char *originalDirectory = NULL;
    int originalDirectorySize = 0;

    // TODO create code for directory change for Windows too...
    originalDirectory = getcwd(originalDirectory, originalDirectorySize);

    unsigned int faultyRepositories = 0;
    vector<string>::const_iterator it=gitRepositoryList.begin();
    vector<string>::const_iterator end=gitRepositoryList.end();
    for (; it!=end; it++)
    {
        int returnValue = chdir(it->c_str());
        if (returnValue != 0)
        {
            ++faultyRepositories;
            continue;
        }


        gitCommand->Run();
        if (gitCommand->GetCommandReturnCode() != 0)
            ++faultyRepositories;
    }

    int backToOriginalDirValue = chdir(originalDirectory);

    JobStatus* status = new JobStatus();
    stringstream descriptionStream;
    if (faultyRepositories == 0)
    {
        if (backToOriginalDirValue == 0)
            status->SetCode(JobStatus::OK);
        else
            status->SetCode(JobStatus::OK_WITH_WARNINGS);
        descriptionStream << gitRepositoryList.size();
        descriptionStream << " Git " << CorrectRepositoryWord(gitRepositoryList.size()) << " updated successfully.";
    }
    else if (faultyRepositories == gitRepositoryList.size())
    {
        status->SetCode(JobStatus::ERROR);
        descriptionStream << gitRepositoryList.size();
        descriptionStream << " Git " << CorrectRepositoryWord(gitRepositoryList.size()) << " failed to update.";
    }
    else
    {
        status->SetCode(JobStatus::OK_WITH_WARNINGS);
        int okRepositories = gitRepositoryList.size()-faultyRepositories;
        descriptionStream << okRepositories;
        descriptionStream << " Git " << CorrectRepositoryWord(gitRepositoryList.size()) << " updated, ";
        descriptionStream << faultyRepositories << " failed.";
    }

    status->SetDescription(descriptionStream.str());
    return status;
}

void GitPullJob::AddRepository(std::string &repositoryPath)
{
    gitRepositoryList.push_back(repositoryPath);
}

void GitPullJob::ClearRepositoryList()
{
    gitRepositoryList.clear();
}

string GitPullJob::CorrectRepositoryWord(int n)
{
    return (n == 1) ? "repository" : "repositories";
}
