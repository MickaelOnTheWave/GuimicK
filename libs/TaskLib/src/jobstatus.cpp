#include "jobstatus.h"

#include <sstream>

using namespace std;


const int JobStatus::NotExecuted		= 0;
const int JobStatus::Ok                = 1;
const int JobStatus::OkWithWarnings  = 2;
const int JobStatus::Error             = 3;

map<int, wstring> JobStatus::codeMap;

JobStatus::JobStatus()
	: code(NotExecuted),
	  elapsedTime(0),
	  description(L"")
{
}

JobStatus::JobStatus(int _code, const wstring &_description)
	: code(_code), description(_description)
{
}

JobStatus::JobStatus(const JobStatus &other)
{
    code = other.code;
    description = other.description;
    elapsedTime = other.elapsedTime;
    copy(other.externalFilenames.begin(), other.externalFilenames.end(), back_inserter(externalFilenames));
    copy(other.filebuffers.begin(), other.filebuffers.end(), back_inserter(filebuffers));
}

JobStatus::~JobStatus()
{
    externalFilenames.clear();
    filebuffers.clear();
}

bool JobStatus::operator==(const JobStatus &other) const
{
   return (code == other.code && elapsedTime == other.elapsedTime &&
           description == other.description && externalFilenames == other.externalFilenames &&
           filebuffers == other.filebuffers);
}

void JobStatus::Reset()
{
	code = NotExecuted;
	description = L"";
   externalFilenames.clear();
   filebuffers.clear();
}

void JobStatus::SetCode(int _code)
{
    code = _code;
}

wstring JobStatus::GetCodeDescription(int _code)
{
   if (codeMap.empty())
      PopulateCodeMap();

   map<int, wstring>::const_iterator it = codeMap.find(_code);
   return (it != codeMap.end()) ? it->second : wstring(L"Unknown status code");
}

int JobStatus::GetCodeFromDescription(const wstring &_description)
{
   if (codeMap.empty())
      PopulateCodeMap();

   map<int, wstring>::const_iterator it = codeMap.begin();
   for (; it != codeMap.end(); ++it)
   {
      if (it->second == _description)
         return it->first;
   }

   return JobStatus::NotExecuted;
}

wstring JobStatus::GetCodeDescription() const
{
    return GetCodeDescription(code);
}

void JobStatus::SetDuration(std::time_t duration)
{
    elapsedTime = duration;
}

void JobStatus::SetDescription(const wstring &_description)
{
	description = _description;
}

void JobStatus::AddExternalFile(const wstring &filename)
{
    externalFilenames.push_back(filename);
}

void JobStatus::AddFileBuffer(const wstring &filename, const wstring &filecontents)
{
   filebuffers.push_back(pair<wstring,wstring>(filename, filecontents));
}

void JobStatus::AddExternalFilesFromStatus(JobStatus *other)
{
    vector<wstring>::const_iterator it=other->externalFilenames.begin();
    for (; it!=other->externalFilenames.end(); ++it)
        AddExternalFile(*it);
}

void JobStatus::AddFileBuffersFromStatus(JobStatus *other)
{
    vector<pair<wstring,wstring> >::const_iterator it=other->filebuffers.begin();
    for (; it!=other->filebuffers.end(); ++it)
        AddFileBuffer(it->first, it->second);
}

void JobStatus::AddAllFilesFromStatus(JobStatus *other)
{
    AddExternalFilesFromStatus(other);
    AddFileBuffersFromStatus(other);
}

void JobStatus::ClearAllFiles()
{
    externalFilenames.clear();
    filebuffers.clear();
}

int JobStatus::GetCode() const
{
	return code;
}

time_t JobStatus::GetDuration() const
{
	return elapsedTime;
}

bool JobStatus::IsWorseThan(int otherCode) const
{
	return code > otherCode;
}

bool JobStatus::IsFatal() const
{
   return (code == Error || code == NotExecuted);
}

bool JobStatus::IsOk() const
{
   return (code == Ok);
}

wstring JobStatus::GetDescription() const
{
	return description;
}

wstring JobStatus::ToString() const
{
	std::wstringstream fullDescription;
	fullDescription << L"Code:" << code << L" - Desc:" << description;
    fullDescription << L" - Files:" << externalFilenames.size();
	return fullDescription.str();
}

bool JobStatus::HasExternalFiles()
{
    return (!externalFilenames.empty());
}

void JobStatus::GetExternalFilenames(vector<wstring> &_fileNames)
{
    vector<wstring>::const_iterator it=externalFilenames.begin();
    for (; it!=externalFilenames.end(); ++it)
        _fileNames.push_back(*it);
}

void JobStatus::GetFileBuffers(FileBufferList &_filebuffers)
{
    FileBufferList::const_iterator it=filebuffers.begin();
    for (; it!=filebuffers.end(); ++it)
       _filebuffers.push_back(*it);
}

void JobStatus::PopulateCodeMap()
{
   codeMap[JobStatus::NotExecuted] = L"Not executed";
   codeMap[JobStatus::Ok] = L"Ok";
   codeMap[JobStatus::OkWithWarnings] = L"Executed with minor errors";
   codeMap[JobStatus::Error] = L"Error";
}
