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

    bool ParseUsingFiles(const std::string& inputFile,
                         const std::string& outputFile, std::string& description);

    BackupReportData *GetRawData();


private:
    bool Parse(const std::string &inputFile);

    long long ParseByteDataLine(const std::string& line,
										 const std::string& wordBefore,
										 const std::string& wordAfter);

    std::string CreateShortReport();
    void CreateFullReport(const std::string& fileName);
    std::string FileListDescription(const std::list<std::string> &fileList, const std::string &operation);


    BackupReportData reportData;

};

#endif // RSNAPSHOTREPORTPARSER_H
