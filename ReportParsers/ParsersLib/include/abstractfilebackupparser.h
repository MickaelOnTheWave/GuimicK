#ifndef ABSTRACTFILEBACKUPPARSER_H
#define ABSTRACTFILEBACKUPPARSER_H

#include "abstractoutputparser.h"
#include "filebackupreport.h"

class AbstractFileBackupParser : public AbstractOutputParser
{
public:
    AbstractFileBackupParser(FileBackupReport* _report);
    virtual ~AbstractFileBackupParser();

    virtual std::string GetMiniDescription();
    virtual std::string GetFullDescription();

    virtual void GetReport(FileBackupReport& reportData) = 0;

protected:
    FileBackupReport* reportData;
};

#endif // ABSTRACTFILEBACKUPPARSER_H
