#ifndef RSNAPSHOTREPORTPARSER_H
#define RSNAPSHOTREPORTPARSER_H

#include "abstractfilebackupparser.h"
#include "filebackupreport.h"

#include <string>
#include <list>


class RsnapshotReport : public FileBackupReport
{
public:
    virtual void Clear();
	std::string BytesTaken() const;
	void CreateModifiedList();

    void operator=(const RsnapshotReport& other);

    virtual std::string GetMiniDescription() const;
    virtual std::string GetFullDescription() const;


	long long bytesAdded;
	long long bytesRemoved;
};

class RSnapshotReportParser : public AbstractFileBackupParser
{
public:
    RSnapshotReportParser();
    virtual ~RSnapshotReportParser();

    virtual bool ParseBuffer(const std::string& buffer);

    virtual void GetReport(FileBackupReport& reportData);


private:
    void ParseLines(const std::vector<std::string>& lines);

    long long ParseByteDataLine(const std::string& line,
										 const std::string& wordBefore,
										 const std::string& wordAfter);

    RsnapshotReport* GetTypedReport();
};

#endif // RSNAPSHOTREPORTPARSER_H
