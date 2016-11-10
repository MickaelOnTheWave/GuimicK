#ifndef RSNAPSHOTREPORTPARSER_H
#define RSNAPSHOTREPORTPARSER_H

#include "abstractoutputparser.h"

#include <string>
#include <list>

class BackupReportData
{
public:
	void Reset();
	std::string BytesTaken() const;
	void CreateModifiedList();

    BackupReportData& operator=(const BackupReportData& other);
    BackupReportData* GetCopy();

	long long bytesAdded;
	long long bytesRemoved;

	std::list<std::string> addedList;
	std::list<std::string> removedList;
	std::list<std::string> modifiedList;
};

class RSnapshotReportParser : public AbstractOutputParser
{
public:
    virtual ~RSnapshotReportParser();

    virtual bool ParseBuffer(const std::string& buffer);
    virtual std::string GetMiniDescription();
    virtual std::string GetFullDescription();

    void GetData(BackupReportData &report);


private:
    bool Parse(const std::string &inputFile);

    long long ParseByteDataLine(const std::string& line,
										 const std::string& wordBefore,
										 const std::string& wordAfter);

    void CreateFullReport(const std::string& fileName);
    std::string FileListDescription(const std::list<std::string> &fileList, const std::string &operation);


    BackupReportData reportData;

};

#endif // RSNAPSHOTREPORTPARSER_H
