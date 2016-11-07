#include "clamavreportparser.h"

#include <fstream>
#include <sstream>

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

}

string ClamAvReportParser::GetMiniDescription()
{

}

string ClamAvReportParser::GetFullDescription()
{

}

bool ClamAvReportParser::ParseUsingFiles(const string &inputFile, const string &/*outputFile*/, string &description)
{
    ClamAvReportData reportData;
    bool returnValue = Parse(inputFile, reportData);

    if (returnValue == false)
    {
        description = "Failed to scan log file.";
    }
    else
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
        description = descriptionStream.str();
    }

    return returnValue;
}

bool ClamAvReportParser::Parse(const std::string& file, ClamAvReportData& data)
{
	data.Reset();

	ifstream inFile(file.c_str());

	if (!inFile.is_open())
		return false;


	string line;

	while (getline(inFile, line))
	{
		if (line[0] != '/')
		{
			// Either a "Scanning" line or an unknown one. Ignore...
			continue;
		}

		data.scannedFilesCount++;
		size_t pos = line.find(':');
		if (pos == string::npos)
		{
			// Strange .. no status? Format changed? Anyway, ignore that...
			continue;
		}

		string status = line.substr(pos+2);
		data.statusMap[status]++;
	}

	inFile.close();
	return true;
}
