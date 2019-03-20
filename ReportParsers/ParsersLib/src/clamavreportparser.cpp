#include "clamavreportparser.h"

#include <fstream>
#include <sstream>
#include <vector>

#include "tools.h"

using namespace std;

ClamAvReportData::ClamAvReportData()
{
	Reset();
}

void ClamAvReportData::Reset()
{
	scannedFilesCount = 0;
	statusMap.clear();
}

bool ClamAvReportParser::ParseBuffer(const wstring &buffer)
{
    reportData.Reset();

    vector<wstring> lines;
    Tools::TokenizeString(buffer, '\n', lines);

    vector<wstring>::const_iterator it=lines.begin();
    for (; it!=lines.end(); ++it)
    {
        if ((*it)[0] != '/')
        {
            // Either a "Scanning" line or an unknown one. Ignore...
            continue;
        }

        reportData.scannedFilesCount++;
        size_t pos = it->find(':');
        if (pos == wstring::npos)
        {
            // Strange .. no status? Format changed? Anyway, ignore that...
            continue;
        }

        wstring status = it->substr(pos+2);
        reportData.statusMap[status]++;
    }

    return true;
}

wstring ClamAvReportParser::GetMiniDescription()
{
    wstringstream descriptionStream;
    descriptionStream << reportData.scannedFilesCount << " files scanned. ";
    map<wstring, unsigned long>::iterator it=reportData.statusMap.begin();
    map<wstring, unsigned long>::iterator end=reportData.statusMap.end();
    for (; it!=end; ++it)
    {
        if (it != reportData.statusMap.begin())
            descriptionStream << ",";

        pair<wstring, unsigned long> current = *it;
        descriptionStream << current.first << ": " << current.second;
    }
    return descriptionStream.str();
}

wstring ClamAvReportParser::GetFullDescription()
{
    return L"Not implemented";
}


