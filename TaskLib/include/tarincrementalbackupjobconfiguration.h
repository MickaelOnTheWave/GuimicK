#ifndef TARINCREMENTALBACKUPJOBCONFIGURATION_H
#define TARINCREMENTALBACKUPJOBCONFIGURATION_H

#include "abstractbackupjobconfiguration.h"

class TarIncrementalBackupJobConfiguration : public AbstractBackupJobConfiguration
{
public:
   TarIncrementalBackupJobConfiguration();

   virtual bool IsRightJob(AbstractJob* job);

protected:
    virtual AbstractJob *CreateJob();

private:
    virtual std::wstring GetBackupItemName() const;

};

#endif // TARINCREMENTALBACKUPJOBCONFIGURATION_H
