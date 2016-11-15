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

    virtual bool ParseBuffer(const std::string& buffer);
    virtual std::string GetMiniDescription();
    virtual std::string GetFullDescription();

private:
    ClamAvReportData reportData;
};

#endif // CLAMAVREPORTPARSER_H
