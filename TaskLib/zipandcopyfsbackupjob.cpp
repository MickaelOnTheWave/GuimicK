#include "zipandcopyfsbackupjob.h"

#include "consolejob.h"
#include "filetools.h"
#include "tarcommandparser.h"

using namespace std;

ZipAndCopyFsBackupJob::ZipAndCopyFsBackupJob()
    : AbstractBackupJob("ZipCopyFsBackup")
{
    statusManager.SetAttachmentName("ZipCopyFsBackup.txt");
    statusManager.SetDebugManager(debugManager);
}

ZipAndCopyFsBackupJob::ZipAndCopyFsBackupJob(const ZipAndCopyFsBackupJob& other)
    : AbstractBackupJob(other)
{
    statusManager.SetAttachmentName("ZipCopyFsBackup.txt");
    statusManager.SetDebugManager(debugManager);
}

std::string ZipAndCopyFsBackupJob::GetName()
{
    return string("ZipAndCopy Backup");
}

AbstractJob *ZipAndCopyFsBackupJob::Clone()
{
    return new ZipAndCopyFsBackupJob(*this);
}

bool ZipAndCopyFsBackupJob::Restore(const string &backupFile, const string &)
{
    const string params = string("-xf ") + backupFile;
    ConsoleJob commandJob("tar", params);
    commandJob.RunWithoutStatus();

    return (commandJob.GetCommandReturnCode() == 0);
}

void ZipAndCopyFsBackupJob::RunRepositoryBackup(const std::string &source,
                                                const std::string &destination,
                                                AbstractBackupJob::ResultCollection &results)
{
    if (isTargetLocal)
    {

        bool ok = PrepareDestination(destination, results);

        if (ok)
            ok = CreateBackupArchive(source, destination, results);

        //if (ok)
        //    MoveBackupArchiveToDestination(destination, results);
    }
    else
    {
        // TODO : implement
        // Run tar -cpzf archive.tar source/ on client
        // copy tar archive to dest
        // remove tar archive from client
    }
}

JobStatus *ZipAndCopyFsBackupJob::CreateGlobalStatus(
        const AbstractBackupJob::ResultCollection &results)
{
    return statusManager.CreateGlobalStatus(results, folderList);
}

bool ZipAndCopyFsBackupJob::CreateBackupArchive(const string &folderToBackup,
                                                const string &archiveName,
                                                AbstractBackupJob::ResultCollection &results)
{
    JobStatus* status = new JobStatus();

    const string params = string("-cpzvf ") + archiveName + " " + folderToBackup;
    ConsoleJob commandJob("tar", params);
    commandJob.RunWithoutStatus();

    if (commandJob.GetCommandReturnCode() == 0)
    {
        TarCommandParser parser(commandJob.GetCommand());
        bool ok = parser.ParseBuffer(commandJob.GetCommandOutput());
        if (ok)
        {
            FileBackupReport* report = new FileBackupReport();
            parser.GetReport(*report);

            status->SetCode(JobStatus::OK);
            results.push_back(make_pair<JobStatus*, FileBackupReport*>(status, report));
        }
        else
        {
            status->SetCode(JobStatus::OK_WITH_WARNINGS);
            status->SetDescription("Failed creating report");
            results.push_back(make_pair<JobStatus*, FileBackupReport*>(status, NULL));
        }
        return true;
    }
    else
    {
        status->SetCode(JobStatus::ERROR);
        status->SetDescription("tar command failed");
        results.push_back(make_pair<JobStatus*, FileBackupReport*>(status, NULL));
        return false;
    }
}

bool ZipAndCopyFsBackupJob::PrepareDestination(const string &destination,
                                               AbstractBackupJob::ResultCollection &results)
{
/*    if (FileTools::FolderExists(destination) == false)
        FileTools::CreateFolder(destination);
*/
    const string finalBackupArchive = destination;// + "/" + archiveName;
    if (FileTools::FileExists(finalBackupArchive))
    {
        // TODO : implement this RemoveFile
        //FileTools::RemoveFile(finalBackupArchive);
        const string params = string("-Rf ") + finalBackupArchive;
        ConsoleJob::Run("rm", params);
    }

    if (!FileTools::FileExists(finalBackupArchive))
        return true;
    else
    {
        JobStatus* status = new JobStatus(JobStatus::ERROR, "error cleaning destination");
        results.push_back(make_pair<JobStatus*, FileBackupReport*>(status, NULL));
        return false;
    }
}
/*
bool ZipAndCopyFsBackupJob::MoveBackupArchiveToDestination(const string &destination,
                                                           AbstractBackupJob::ResultCollection &results)
{
    string params = archiveName + " " + destination + "/";

    ConsoleJob commandJob("mv", params);
    commandJob.RunWithoutStatus();

    if (commandJob.GetCommandReturnCode() == 0)
        return true;
    else
    {
        JobStatus* status = new JobStatus(JobStatus::ERROR, "failed to move archive");
        results.push_back(make_pair<JobStatus*, FileBackupReport*>(status, NULL));
        return false;
    }
}*/
