#include "rsnapshotreportparser.h"

#include <algorithm>
#include <fstream>
#include <sstream>

#include <tools.h>

using namespace std;

void RsnapshotReport::Clear()
{
    FileBackupReport::Clear();

	bytesAdded = 0;
	bytesRemoved = 0;
}

std::string RsnapshotReport::BytesTaken() const
{
	long long bytesTaken = (bytesAdded - bytesRemoved);

    std::string formattedQuantity;
	if (bytesTaken < 0)
		formattedQuantity = Tools::FormatByteString(-bytesTaken) + " freed";
	else
		formattedQuantity = Tools::FormatByteString(bytesTaken) + " taken";

	return formattedQuantity;
}

void RsnapshotReport::CreateModifiedList()
{
    vector<string>::iterator addedIt=added.begin();
    vector<string>::iterator addedEnd=added.end();
	while (addedIt != addedEnd)
	{
        vector<string>::iterator removedIt = find(removed.begin(), removed.end(), *addedIt);
        if (removedIt != removed.end())
		{
            modified.push_back(*addedIt);
            removed.erase(removedIt);
            addedIt = added.erase(addedIt);
		}
		else
			addedIt++;
    }
}

void RsnapshotReport::operator=(const RsnapshotReport &other)
{
    FileBackupReport::operator =(other);

    bytesAdded    = other.bytesAdded;
    bytesRemoved  = other.bytesRemoved;
}

//----------------------------------------------------------------------------

bool RSnapshotReportParser::Parse(const std::string &inputFile)
{
    reportData.Clear();

    std::ifstream inFile(inputFile.c_str());

	if (inFile.is_open())
	{
        std::string line;

		while (getline(inFile, line) && line.find("Comparing") != 0)
		{
		}

		while (getline(inFile, line) && line.find("Between") != 0)
		{
			size_t posSimbol = line.find_first_of("+-");
            if (posSimbol == std::string::npos)
				continue;

			char simbol = line[posSimbol];
			size_t posFile = line.find_first_not_of(" ", posSimbol+1);
            if (posFile == std::string::npos)
				continue;

            std::string backupFileName(line.substr(posFile+1));

			size_t weeklyPos = backupFileName.find("weekly.");
            if (weeklyPos != std::string::npos)
				weeklyPos = backupFileName.find("/", weeklyPos);

            std::string fileName;
            if (weeklyPos != std::string::npos)
				fileName = backupFileName.substr(weeklyPos);
			else
				fileName = backupFileName;

			if (simbol == '+')
                reportData.added.push_back(fileName);
			else // simbol == '-'
                reportData.removed.push_back(fileName);
		}

		bool foundAddedBytesString = false;
		bool foundRemovedBytesString = false;
		while (getline(inFile, line) && (!foundAddedBytesString || !foundRemovedBytesString))
		{
			if (!foundAddedBytesString &&
                 line.find("added") != std::string::npos && line.find("taking") != std::string::npos)
			{
				reportData.bytesAdded = ParseByteDataLine(line, "taking", "bytes");
				foundAddedBytesString = true;
			}

			if (!foundRemovedBytesString &&
                 line.find("removed") != std::string::npos && line.find("saving") != std::string::npos)
			{
				reportData.bytesRemoved = ParseByteDataLine(line, "saving", "bytes");
				foundRemovedBytesString = true;
			}
		}

		inFile.close();
	}
	else
		return false;

	reportData.CreateModifiedList();

    return true;
}

RSnapshotReportParser::~RSnapshotReportParser()
{
}

bool RSnapshotReportParser::ParseBuffer(const std::string &buffer)
{

}

std::string RSnapshotReportParser::GetMiniDescription()
{
    std::stringstream description;
    description << reportData.added.size() << " added, ";
    description << reportData.modified.size() << " modified, ";
    description << reportData.removed.size() << " removed. ";
    description << reportData.BytesTaken();
    return description.str();
}

std::string RSnapshotReportParser::GetFullDescription()
{

}

void RSnapshotReportParser::GetReport(FileBackupReport &report)
{
    RsnapshotReport* typedReport = dynamic_cast<RsnapshotReport*>(&report);
    if (typedReport == NULL)
        return;

    *typedReport = reportData;
}

/*
bool RSnapshotReportParser::ParseUsingFiles(const std::string &inputFile, const std::string &outputFile, std::string &description)
{
    description = "";

    bool returnVal = Parse(inputFile);

    if (returnVal)
    {
        description = CreateShortReport();
        CreateFullReport(outputFile);
    }

    return returnVal;
    
}*/

long long RSnapshotReportParser::ParseByteDataLine(const std::string &line, const std::string &wordBefore, const std::string &wordAfter)
{
	long long bytes = 0;
	size_t initPos = line.find(wordBefore)+wordBefore.size();
	size_t endPos = line.rfind(wordAfter);
    std::string byteString(line.substr(initPos, endPos));
    std::istringstream is (byteString);
	is >> bytes;
	return bytes;
}

void RSnapshotReportParser::CreateFullReport(const std::string &fileName)
{
    std::ofstream outFile(fileName.c_str());
    outFile << FileListDescription(reportData.added, "added");
    outFile << FileListDescription(reportData.modified, "modified");
    outFile << FileListDescription(reportData.removed, "removed");
    outFile << Tools::FormatByteString(reportData.bytesAdded) << " added, ";
    outFile << Tools::FormatByteString(reportData.bytesRemoved) << " removed, and overall ";
    outFile << reportData.BytesTaken() << std::endl;
    outFile.close();
}

std::string RSnapshotReportParser::FileListDescription(const vector<string>& fileList,
                                                       const string& operation)
{
    stringstream description;
    description << fileList.size() << " files " << operation << std::endl;

    vector<string>::const_iterator it=fileList.begin();
    vector<string>::const_iterator end=fileList.end();
    for (; it!=end; it++)
        description << "\t" << *it << std::endl;
    description << std::endl;

    return description.str();
}
