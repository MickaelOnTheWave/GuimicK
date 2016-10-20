#ifndef CLAMAVREPORTPARSER_H
#define CLAMAVREPORTPARSER_H

#include "abstractoutputparser.h"
#include <map>
#include <string>

class ClamAvReportData
{
public:
	ClamAvReportData();

	void Reset();

	unsigned long scannedFilesCount;
	std::map<std::string, unsigned long> statusMap;
};

class ClamAvReportParser : public AbstractOutputParser
{
public:
    virtual bool ParseUsingFiles(const std::string& inputFile,
                                 const std::string& outputFile, std::string& description);

private:
    bool Parse(const std::string& file, ClamAvReportData& data);
};

#endif // CLAMAVREPORTPARSER_H
