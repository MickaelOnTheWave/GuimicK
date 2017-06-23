#include "zipandcopyfsbackupjob.h"

#include "consolejob.h"
#include "filetools.h"

using namespace std;

const string archiveName = "backup.tar";

ZipAndCopyFsBackupJob::ZipAndCopyFsBackupJob() : AbstractBackupJob("ZipCopyFsBackup")
{
}

ZipAndCopyFsBackupJob::ZipAndCopyFsBackupJob(const ZipAndCopyFsBackupJob& other)
    : AbstractBackupJob(other)
{
}

std::string ZipAndCopyFsBackupJob::GetName()
{
    return string("ZipAndCopy Backup");
}

AbstractJob *ZipAndCopyFsBackupJob::Clone()
{
    return new ZipAndCopyFsBackupJob(*this);
}

bool ZipAndCopyFsBackupJob::Restore(const string &backupFile, const string &destination)
{
    // Run tar -xf archive.tar
    return false;
}

void ZipAndCopyFsBackupJob::RunRepositoryBackup(const std::string &source,
                                                const std::string &destination,
                                                AbstractBackupJob::ResultCollection &results)
{
    if (isTargetLocal)
    {
        bool ok = CreateBackupArchive(source, results);

        if (ok)
            PrepareDestination(destination, results);

        if (ok)
            MoveBackupArchiveToDestination(destination, results);
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
    // TODO : implement
    return NULL;
}

bool ZipAndCopyFsBackupJob::CreateBackupArchive(const string &folderToBackup,
                                                AbstractBackupJob::ResultCollection &results)
{
    // TODO : finish implementation
    const string params = string("-cpzvf ") + archiveName + " " + folderToBackup;
    ConsoleJob::Run("tar", params);
    return true;
}

bool ZipAndCopyFsBackupJob::PrepareDestination(const string &destination,
                                               AbstractBackupJob::ResultCollection &results)
{
    // TODO : finish implementation
    if (FileTools::FolderExists(destination) == false)
        FileTools::CreateFolder(destination);
    const string finalBackupArchive = destination + "/" + archiveName;
    if (FileTools::FileExists(finalBackupArchive))
    {
        // TODO : implement this RemoveFile
        //FileTools::RemoveFile(finalBackupArchive);
        const string params = string("-Rf ") + finalBackupArchive;
        ConsoleJob::Run("rm", params);
    }
    return true;
}

bool ZipAndCopyFsBackupJob::MoveBackupArchiveToDestination(const string &destination,
                                                           AbstractBackupJob::ResultCollection &results)
{
    const string params = archiveName + " " + destination + "/";
    ConsoleJob::Run("mv", params);
}
