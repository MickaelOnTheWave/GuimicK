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
	std::map<std::wstring, unsigned long> statusMap;
};

class ClamAvReportParser : public AbstractOutputParser
{
public:

    virtual bool ParseBuffer(const std::wstring& buffer);
    virtual std::wstring GetMiniDescription();
    virtual std::wstring GetFullDescription();

private:
    ClamAvReportData reportData;
};

#endif // CLAMAVREPORTPARSER_H
