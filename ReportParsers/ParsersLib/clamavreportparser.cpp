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

bool ClamAvReportParser::ParseBuffer(const string &buffer)
{
    reportData.Reset();

    vector<string> lines;
    Tools::TokenizeString(buffer, '\n', lines);

    vector<string>::const_iterator it=lines.begin();
    for (; it!=lines.end(); ++it)
    {
        if ((*it)[0] != '/')
        {
            // Either a "Scanning" line or an unknown one. Ignore...
            continue;
        }

        reportData.scannedFilesCount++;
        size_t pos = it->find(':');
        if (pos == string::npos)
        {
            // Strange .. no status? Format changed? Anyway, ignore that...
            continue;
        }

        string status = it->substr(pos+2);
        reportData.statusMap[status]++;
    }

    return true;
}

string ClamAvReportParser::GetMiniDescription()
{
    stringstream descriptionStream;
    descriptionStream << reportData.scannedFilesCount << " files scanned. ";
    map<string, unsigned long>::iterator it=reportData.statusMap.begin();
    map<string, unsigned long>::iterator end=reportData.statusMap.end();
    for (; it!=end; ++it)
    {
        if (it != reportData.statusMap.begin())
            descriptionStream << ",";

        pair<string, unsigned long> current = *it;
        descriptionStream << current.first << ": " << current.second;
    }
    return descriptionStream.str();
}

string ClamAvReportParser::GetFullDescription()
{
    return string("Not implemented");
}


