#include "abstractfsbackupjobconfiguration.h"

using namespace std;

AbstractFsBackupJobConfiguration::AbstractFsBackupJobConfiguration(const wstring &tag)
    : AbstractBackupJobConfiguration(tag)
{
}

wstring AbstractFsBackupJobConfiguration::GetBackupItemName() const
{
    return L"Folder";
}
