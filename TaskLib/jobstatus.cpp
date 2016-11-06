#include "jobstatus.h"

#include <sstream>

using namespace std;

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

JobStatus::~JobStatus()
{
	filenames.clear();
}

void JobStatus::Reset()
{
	code = NOT_EXECUTED;
	description = "";
	filenames.clear();
}

void JobStatus::SetCode(int _code)
{
    code = _code;
}

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

void JobStatus::AddFile(const string &filename)
{
    filenames.push_back(filename);
}

void JobStatus::AddFilesFromStatus(JobStatus *other)
{
    vector<string> files;
    other->GetFilenames(files);
    vector<string>::const_iterator it=files.begin();
    for (; it!=files.end(); ++it)
        AddFile(*it);
}

void JobStatus::RemoveAllFiles()
{
    filenames.clear();
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
	fullDescription << " - Files:" << filenames.size();
	return fullDescription.str();
}

bool JobStatus::HasFiles()
{
	return (!filenames.empty());
}

void JobStatus::GetFilenames(vector<string> &_fileNames)
{
	vector<string>::const_iterator it=filenames.begin();
	vector<string>::const_iterator end=filenames.end();
	for (; it!=end; it++)
		_fileNames.push_back(*it);
}
