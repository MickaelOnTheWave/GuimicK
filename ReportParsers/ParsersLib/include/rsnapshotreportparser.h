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
	std::wstring BytesTaken() const;
	void CreateModifiedList();

    void operator=(const RsnapshotReport& other);

    virtual std::wstring GetMiniDescription() const;
    virtual std::wstring GetFullDescription() const;

	long long bytesAdded;
	long long bytesRemoved;
};

class RSnapshotReportParser : public AbstractFileBackupParser
{
public:
    RSnapshotReportParser();
    virtual ~RSnapshotReportParser();

    virtual bool ParseBuffer(const std::wstring& buffer);

    virtual void GetReport(FileBackupReport& reportData);


private:
    void ParseLines(const std::vector<std::wstring>& lines);

    long long ParseByteDataLine(const std::wstring& line,
										 const std::wstring& wordBefore,
										 const std::wstring& wordAfter);

    RsnapshotReport* GetTypedReport();
};

#endif // RSNAPSHOTREPORTPARSER_H
