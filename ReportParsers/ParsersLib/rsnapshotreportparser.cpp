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

string RsnapshotReport::GetMiniDescription() const
{
    stringstream description;
    description << FileBackupReport::GetMiniDescription() << ". ";
    description << BytesTaken();
    return description.str();
}

string RsnapshotReport::GetFullDescription() const
{
    stringstream description;
    description << FileBackupReport::GetFullDescription();
    description << Tools::FormatByteString(bytesAdded) << " added, ";
    description << Tools::FormatByteString(bytesRemoved) << " removed, and overall ";
    description << BytesTaken() << std::endl;
    return description.str();
}

//----------------------------------------------------------------------------

RSnapshotReportParser::RSnapshotReportParser()
    : AbstractFileBackupParser(new RsnapshotReport())
{
}

RSnapshotReportParser::~RSnapshotReportParser()
{
}

bool RSnapshotReportParser::ParseBuffer(const std::string &buffer)
{
    reportData->Clear();

    vector<string> lines;
    Tools::TokenizeString(buffer, '\n', lines);

    // TODO : refactor inside this (very messy) method and here too.
    ParseLines(lines);

    GetTypedReport()->CreateModifiedList();
    reportData->SortData();

    return true;
}

void RSnapshotReportParser::GetReport(FileBackupReport &report)
{
    RsnapshotReport* typedReport = dynamic_cast<RsnapshotReport*>(&report);
    if (typedReport == NULL)
        return;

    *typedReport = *GetTypedReport();
}

void RSnapshotReportParser::ParseLines(const std::vector<string> &lines)
{
    vector<string>::const_iterator it=lines.begin();

    while (it != lines.end() && it->find("Comparing") != 0)
        ++it;

    while (it != lines.end() && it->find("Between") != 0)
    {
        size_t posSimbol = it->find_first_of("+-");
        if (posSimbol != std::string::npos)
        {
            char simbol = (*it)[posSimbol];
            size_t posFile = it->find_first_not_of(" ", posSimbol+1);
            if (posFile != std::string::npos)
            {
                std::string backupFileName(it->substr(posFile+1));

                size_t weeklyPos = backupFileName.find("weekly.");
                if (weeklyPos != std::string::npos)
                    weeklyPos = backupFileName.find("/", weeklyPos);

                std::string fileName;
                if (weeklyPos != std::string::npos)
                    fileName = backupFileName.substr(weeklyPos);
                else
                    fileName = backupFileName;

                if (simbol == '+')
                    reportData->AddAsAdded(fileName);
                else // simbol == '-'
                    reportData->AddAsRemoved(fileName);
            }
        }
        ++it;
    }

    bool foundAddedBytesString = false;
    bool foundRemovedBytesString = false;
    while (it != lines.end() && (!foundAddedBytesString || !foundRemovedBytesString))
    {
        if (!foundAddedBytesString &&
             it->find("added") != std::string::npos && it->find("taking") != std::string::npos)
        {
            GetTypedReport()->bytesAdded = ParseByteDataLine(*it, "taking", "bytes");
            foundAddedBytesString = true;
        }

        if (!foundRemovedBytesString &&
             it->find("removed") != std::string::npos && it->find("saving") != std::string::npos)
        {
            GetTypedReport()->bytesRemoved = ParseByteDataLine(*it, "saving", "bytes");
            foundRemovedBytesString = true;
        }
        ++it;
    }
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

RsnapshotReport *RSnapshotReportParser::GetTypedReport()
{
    return static_cast<RsnapshotReport*>(reportData);
}

