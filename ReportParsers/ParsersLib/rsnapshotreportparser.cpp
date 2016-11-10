#include "rsnapshotreportparser.h"

#include <algorithm>
#include <fstream>
#include <sstream>

#include <tools.h>

void BackupReportData::Reset()
{
	bytesAdded = 0;
	bytesRemoved = 0;
	addedList.clear();
	removedList.clear();
	modifiedList.clear();
}

std::string BackupReportData::BytesTaken() const
{
	long long bytesTaken = (bytesAdded - bytesRemoved);

    std::string formattedQuantity;
	if (bytesTaken < 0)
		formattedQuantity = Tools::FormatByteString(-bytesTaken) + " freed";
	else
		formattedQuantity = Tools::FormatByteString(bytesTaken) + " taken";

	return formattedQuantity;
}

void BackupReportData::CreateModifiedList()
{
    std::list<std::string>::iterator addedIt=addedList.begin();
    std::list<std::string>::iterator addedEnd=addedList.end();
	while (addedIt != addedEnd)
	{
        std::list<std::string>::iterator removedIt = find(removedList.begin(), removedList.end(), *addedIt);
		if (removedIt != removedList.end())
		{
			modifiedList.push_back(*addedIt);
			removedList.erase(removedIt);
			addedIt = addedList.erase(addedIt);
		}
		else
			addedIt++;
    }
}

BackupReportData &BackupReportData::operator=(const BackupReportData &other)
{
    bytesAdded    = other.bytesAdded;
    bytesRemoved  = other.bytesRemoved;

    addedList.clear();
    std::copy(other.addedList.begin(), other.addedList.end(), addedList.begin());

    removedList.clear();
    std::copy(other.removedList.begin(), other.removedList.end(), removedList.begin());

    modifiedList.clear();
    std::copy(other.modifiedList.begin(), other.modifiedList.end(), modifiedList.begin());

    return *this;
}

BackupReportData *BackupReportData::GetCopy()
{
    BackupReportData* copy = new BackupReportData();

    copy->bytesAdded    = bytesAdded;
    copy->bytesRemoved  = bytesRemoved;

    copy->addedList.clear();
    std::copy(addedList.begin(), addedList.end(), copy->addedList.begin());

    copy->removedList.clear();
    std::copy(removedList.begin(), removedList.end(), copy->removedList.begin());

    copy->modifiedList.clear();
    std::copy(modifiedList.begin(), modifiedList.end(), copy->modifiedList.begin());

    return copy;
}


bool RSnapshotReportParser::Parse(const std::string &inputFile)
{
	reportData.Reset();

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
				reportData.addedList.push_back(fileName);
			else // simbol == '-'
				reportData.removedList.push_back(fileName);
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
    description << reportData.addedList.size() << " added, ";
    description << reportData.modifiedList.size() << " modified, ";
    description << reportData.removedList.size() << " removed. ";
    description << reportData.BytesTaken();
    return description.str();
}

std::string RSnapshotReportParser::GetFullDescription()
{

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

void RSnapshotReportParser::GetData(BackupReportData& report)
{
    report = reportData;
}

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
    outFile << FileListDescription(reportData.addedList, "added");
    outFile << FileListDescription(reportData.modifiedList, "modified");
    outFile << FileListDescription(reportData.removedList, "removed");
    outFile << Tools::FormatByteString(reportData.bytesAdded) << " added, ";
    outFile << Tools::FormatByteString(reportData.bytesRemoved) << " removed, and overall ";
    outFile << reportData.BytesTaken() << std::endl;
    outFile.close();
}

std::string RSnapshotReportParser::FileListDescription(const std::list<std::string>& fileList, const std::string& operation)
{
    std::stringstream description;
    description << fileList.size() << " files " << operation << std::endl;

    std::list<std::string>::const_iterator it=fileList.begin();
    std::list<std::string>::const_iterator end=fileList.end();
    for (; it!=end; it++)
        description << "\t" << *it << std::endl;
    description << std::endl;

    return description.str();
}
