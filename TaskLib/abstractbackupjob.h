#ifndef ABSTRACTBACKUPJOB_H
#define ABSTRACTBACKUPJOB_H

#include "abstractjob.h"

class AbstractBackupJob : public AbstractJob
{
public:
    AbstractBackupJob();
    AbstractBackupJob(const AbstractBackupJob& other);

    virtual bool InitializeFromClient(Client* client);
    virtual bool IsInitialized(void);

    void SetTargetRemote(const std::string& user = "", const std::string& host = "");
    void SetTargetLocal();

    void SetRepository(const std::string& value);
    void AddFolder(const std::string& source, const std::string& destination);
    void GetFolderList(std::vector<std::pair<std::string, std::string> >& folders);
    void ClearFolderList(void);

protected:
    std::string repository;
    std::vector<std::pair<std::string, std::string> > folderList;

    std::string sshUser;
    std::string sshHost;
    bool isTargetLocal;

private:
    bool IsRemoteTargetConsistent() const;
};
#endif // ABSTRACTBACKUPJOB_H
