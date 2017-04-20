#include "abstractfilebackupparser.h"

AbstractFileBackupParser::AbstractFileBackupParser(FileBackupReport *_report)
    : reportData(_report)
{
}

AbstractFileBackupParser::~AbstractFileBackupParser()
{
    delete reportData;
}

std::string AbstractFileBackupParser::GetMiniDescription()
{
    return reportData->GetMiniDescription();
}

std::string AbstractFileBackupParser::GetFullDescription()
{
    return reportData->GetFullDescription();
}

