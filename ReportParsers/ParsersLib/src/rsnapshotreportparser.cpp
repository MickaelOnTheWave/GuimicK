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

wstring RsnapshotReport::BytesTaken() const
{
	long long bytesTaken = (bytesAdded - bytesRemoved);

    std::wstring formattedQuantity;
	if (bytesTaken < 0)
		formattedQuantity = Tools::FormatByteString(-bytesTaken) + L" freed";
	else
		formattedQuantity = Tools::FormatByteString(bytesTaken) + L" taken";

	return formattedQuantity;
}

void RsnapshotReport::CreateModifiedList()
{
    vector<wstring>::iterator addedIt=added.begin();
    vector<wstring>::iterator addedEnd=added.end();
	while (addedIt != addedEnd)
	{
        vector<wstring>::iterator removedIt = find(removed.begin(), removed.end(), *addedIt);
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

wstring RsnapshotReport::GetMiniDescription() const
{
    wstringstream description;
    description << FileBackupReport::GetMiniDescription() << ". ";
    description << BytesTaken();
    return description.str();
}

wstring RsnapshotReport::GetFullDescription() const
{
    wstringstream description;
    description << FileBackupReport::GetFullDescription();
    description << Tools::FormatByteString(bytesAdded) << L" added, ";
    description << Tools::FormatByteString(bytesRemoved) << L" removed, and overall ";
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

bool RSnapshotReportParser::ParseBuffer(const std::wstring &buffer)
{
    reportData->Clear();

    vector<wstring> lines;
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

void RSnapshotReportParser::ParseLines(const std::vector<wstring> &lines)
{
    vector<wstring>::const_iterator it=lines.begin();

    while (it != lines.end() && it->find(L"Comparing") != 0)
        ++it;

    while (it != lines.end() && it->find(L"Between") != 0)
    {
        size_t posSimbol = it->find_first_of(L"+-");
        if (posSimbol != std::wstring::npos)
        {
            char simbol = (*it)[posSimbol];
            size_t posFile = it->find_first_not_of(L" ", posSimbol+1);
            if (posFile != std::wstring::npos)
            {
                std::wstring backupFileName(it->substr(posFile+1));

                size_t weeklyPos = backupFileName.find(L"weekly.");
                if (weeklyPos != std::wstring::npos)
                    weeklyPos = backupFileName.find(L"/", weeklyPos);

                std::wstring fileName;
                if (weeklyPos != std::wstring::npos)
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

    bool foundAddedByteswstring = false;
    bool foundRemovedByteswstring = false;
    while (it != lines.end() && (!foundAddedByteswstring || !foundRemovedByteswstring))
    {
        if (!foundAddedByteswstring &&
             it->find(L"added") != std::wstring::npos && it->find(L"taking") != std::wstring::npos)
        {
            GetTypedReport()->bytesAdded = ParseByteDataLine(*it, L"taking", L"bytes");
            foundAddedByteswstring = true;
        }

        if (!foundRemovedByteswstring &&
             it->find(L"removed") != std::wstring::npos && it->find(L"saving") != std::wstring::npos)
        {
            GetTypedReport()->bytesRemoved = ParseByteDataLine(*it, L"saving", L"bytes");
            foundRemovedByteswstring = true;
        }
        ++it;
    }
}

long long RSnapshotReportParser::ParseByteDataLine(const std::wstring &line, const std::wstring &wordBefore, const std::wstring &wordAfter)
{
	long long bytes = 0;
	size_t initPos = line.find(wordBefore)+wordBefore.size();
	size_t endPos = line.rfind(wordAfter);
    wstring bytewstring(line.substr(initPos, endPos));
    wistringstream is (bytewstring);
	is >> bytes;
    return bytes;
}

RsnapshotReport *RSnapshotReportParser::GetTypedReport()
{
    return static_cast<RsnapshotReport*>(reportData);
}

