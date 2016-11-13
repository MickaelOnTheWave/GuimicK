#ifndef ABSTRACTFILEBACKUPPARSER_H
#define ABSTRACTFILEBACKUPPARSER_H

#include "abstractoutputparser.h"
#include "filebackupreport.h"

class AbstractFileBackupParser : public AbstractOutputParser
{
public:
    virtual ~AbstractFileBackupParser();
    virtual void GetReport(FileBackupReport& report) = 0;
};

#endif // ABSTRACTFILEBACKUPPARSER_H
