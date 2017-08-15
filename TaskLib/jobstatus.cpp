#include "jobstatus.h"

#include <sstream>

using namespace std;


const int JobStatus::NOT_EXECUTED		= 0;
const int JobStatus::OK                 = 1;
const int JobStatus::OK_WITH_WARNINGS   = 2;
const int JobStatus::ERROR				= 3;

JobStatus::JobStatus()
	: code(NOT_EXECUTED),
	  elapsedTime(0),
	  description("")
{
}

JobStatus::JobStatus(int _code, const string &_description)
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

void JobStatus::Reset()
{
	code = NOT_EXECUTED;
	description = "";
    externalFilenames.clear();
    filebuffers.clear();
}

void JobStatus::SetCode(int _code)
{
    code = _code;
}

// TODO : use a static map instead of these if/else chains
string JobStatus::GetCodeDescription(int _code)
{
    if (_code == JobStatus::NOT_EXECUTED)
        return "Not executed";
    else if (_code == JobStatus::OK)
        return "Ok";
    else if (_code == JobStatus::ERROR)
        return "Error";
    if (_code == JobStatus::OK_WITH_WARNINGS)
        return "Executed with minor errors";
    else
        return "Unknown status code";
}

int JobStatus::GetCodeFromDescription(const string &_description)
{
    if (_description == "Not executed")
        return JobStatus::NOT_EXECUTED;
    else if (_description == "Ok")
        return JobStatus::OK;
    else if (_description == "Error")
        return JobStatus::ERROR;
    else if (_description == "Executed with minor errors")
        return JobStatus::OK_WITH_WARNINGS;
    else
        return JobStatus::NOT_EXECUTED;
}

string JobStatus::GetCodeDescription() const
{
    return GetCodeDescription(code);
}

void JobStatus::SetDuration(std::time_t duration)
{
    elapsedTime = duration;
}

void JobStatus::SetDescription(const string &_description)
{
	description = _description;
}

void JobStatus::AddExternalFile(const string &filename)
{
    externalFilenames.push_back(filename);
}

void JobStatus::AddFileBuffer(const string &filename, const string &filecontents)
{
    filebuffers.push_back(make_pair<string,string>(filename, filecontents));
}

void JobStatus::AddExternalFilesFromStatus(JobStatus *other)
{
    vector<string>::const_iterator it=other->externalFilenames.begin();
    for (; it!=other->externalFilenames.end(); ++it)
        AddExternalFile(*it);
}

void JobStatus::AddFileBuffersFromStatus(JobStatus *other)
{
    vector<pair<string,string> >::const_iterator it=other->filebuffers.begin();
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
	return (code == ERROR || code == NOT_EXECUTED);
}

string JobStatus::GetDescription() const
{
	return description;
}

string JobStatus::ToString() const
{
	std::stringstream fullDescription;
	fullDescription << "Code:" << code << " - Desc:" << description;
    fullDescription << " - Files:" << externalFilenames.size();
	return fullDescription.str();
}

bool JobStatus::HasExternalFiles()
{
    return (!externalFilenames.empty());
}

void JobStatus::GetExternalFilenames(vector<string> &_fileNames)
{
    vector<string>::const_iterator it=externalFilenames.begin();
    for (; it!=externalFilenames.end(); ++it)
        _fileNames.push_back(*it);
}

void JobStatus::GetFileBuffers(FileBufferList &_filebuffers)
{
    FileBufferList::const_iterator it=filebuffers.begin();
    for (; it!=filebuffers.end(); ++it)
        _filebuffers.push_back(*it);
}
