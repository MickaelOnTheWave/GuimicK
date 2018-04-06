#ifndef TARINCREMENTALBACKUPJOBCONFIGURATIONTEST_H
#define TARINCREMENTALBACKUPJOBCONFIGURATIONTEST_H

#include "abstractbackupjobconfigurationtest.h"

class TarIncrementalBackupJobConfigurationTest : public AbstractBackupJobConfigurationTest
{
   Q_OBJECT

public:
   TarIncrementalBackupJobConfigurationTest() = default;
   virtual ~TarIncrementalBackupJobConfigurationTest() = default;

private:
   virtual AbstractJobConfiguration* CreateNewConfiguration() const override;
};

#endif // TARINCREMENTALBACKUPJOBCONFIGURATIONTEST_H
