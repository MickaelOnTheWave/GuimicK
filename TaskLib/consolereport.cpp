#include "consolereport.h"

#include <tools.h>

using namespace std;

TextReportCreator::TextReportCreator()
	: report(""), version(""), generalCode(JobStatus::NOT_EXECUTED),
	  generalDuration(0)
{
}

std::string TextReportCreator::Generate(WorkResultData *data, const string &versionString)
{
	report.str("");
	version = versionString;

	vector<pair<string, ClientJobResults*> >::iterator itClient=data->allClientsResults.begin();
	vector<pair<string, ClientJobResults*> >::iterator endClient=data->allClientsResults.end();
	for (; itClient!=endClient; itClient++)
	{
		pair<string, ClientJobResults*> clientData = *itClient;
		AddClientInformation(clientData.first);

		unsigned int nameCellSize = FindBiggestJobNameSize(clientData.second)+3;

		ClientJobResults::iterator itJob=clientData.second->begin();
		ClientJobResults::iterator endJob=clientData.second->end();
		for (; itJob!=endJob; itJob++)
		{
			pair<string, JobStatus*> jobData = *itJob;
			AddJobInformation(jobData.first, jobData.second, nameCellSize);
		}

		FinishReportGeneration(nameCellSize);
	}
	return report.str();
}

void TextReportCreator::AddClientInformation(const std::string &clientName)
{
	report << "\t\t\t" << clientName << endl;
	report << endl;
	report << endl;
}

void TextReportCreator::AddJobInformation(const std::string &jobName, JobStatus *jobStatus, unsigned int nameCellSize)
{
	int localCode = jobStatus->GetCode();
	string stringOutput(jobStatus->GetDescription());
    jobStatus->GetExternalFilenames(jobsFilenames);

	generalDuration += jobStatus->GetDuration();

	if (jobStatus->IsWorseThan(generalCode))
		generalCode = localCode;

    report << "\t" << jobName << SpacingString(nameCellSize-jobName.size()) << jobStatus->GetCodeDescription();
	report << "\t" << Tools::FormatTimeString(jobStatus->GetDuration()) << endl;

	if (stringOutput != "")
		report << "\t\t" << stringOutput << endl;

}

void TextReportCreator::FinishReportGeneration(unsigned int nameCellSize)
{
	report << endl;
    report << "\t" << "Overall" << SpacingString(nameCellSize-string("Overall").size()) << JobStatus::GetCodeDescription(generalCode);
	report << "\t" << Tools::FormatTimeString(generalDuration) << endl;
	report << endl;
    report << "Task Manager version " << version << endl;
}

unsigned int TextReportCreator::FindBiggestJobNameSize(ClientJobResults *data)
{
	unsigned int biggestSize = string("Overall").size();

	ClientJobResults::iterator itJob=data->begin();
	ClientJobResults::iterator endJob=data->end();
	for (; itJob!=endJob; itJob++)
	{
		pair<string, JobStatus*> jobData = *itJob;
		if (jobData.first.size() > biggestSize)
			biggestSize = jobData.first.size();
	}
	return biggestSize;
}

string TextReportCreator::SpacingString(unsigned int spacesToFill)
{
	string spacingString;
	for (unsigned int i=0; i<spacesToFill; i++)
		spacingString += " ";
	return spacingString;
}

void TextReportCreator::GetAssociatedFiles(list<string> &fileList)
{
	fileList.clear();
	vector<string>::iterator it=jobsFilenames.begin();
	vector<string>::iterator end=jobsFilenames.end();
	for (; it!=end; it++)
		fileList.push_back(*it);
}
