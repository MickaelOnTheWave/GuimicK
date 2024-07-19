#include "abstractfilebackupparser.h"

AbstractFileBackupParser::AbstractFileBackupParser(FileBackupReport *_report)
    : reportData(_report)
{
}

AbstractFileBackupParser::~AbstractFileBackupParser()
{
    delete reportData;
}

std::wstring AbstractFileBackupParser::GetMiniDescription()
{
    return reportData->GetMiniDescription();
}

std::wstring AbstractFileBackupParser::GetFullDescription()
{
    return reportData->GetFullDescription();
}

