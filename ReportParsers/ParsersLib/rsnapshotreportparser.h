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

	long long bytesAdded;
	long long bytesRemoved;
};

class RSnapshotReportParser : public AbstractFileBackupParser
{
public:
    virtual ~RSnapshotReportParser();

    virtual bool ParseBuffer(const std::string& buffer);
    virtual std::string GetMiniDescription();
    virtual std::string GetFullDescription();

    virtual void GetReport(FileBackupReport& report);


private:
    void ParseLines(const std::vector<std::string>& lines);

    long long ParseByteDataLine(const std::string& line,
										 const std::string& wordBefore,
										 const std::string& wordAfter);

    std::string FileListDescription(const std::vector<std::string> &fileList,
                                    const std::string &operation);


    RsnapshotReport reportData;

};

#endif // RSNAPSHOTREPORTPARSER_H
