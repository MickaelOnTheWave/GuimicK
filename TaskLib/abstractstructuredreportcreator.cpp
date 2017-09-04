#include "abstractstructuredreportcreator.h"

using namespace std;

AbstractStructuredReportCreator::AbstractStructuredReportCreator()
{
}

AbstractStructuredReportCreator::~AbstractStructuredReportCreator()
{
}

void AbstractStructuredReportCreator::Generate(WorkResultData *data,
                                               const vector<string> &configErrors,
                                               const string &version)
{
    report.str("");

    AddHeader();

    vector<pair<string, ClientJobResults*> >::iterator itClient=data->allClientsResults.begin();
    vector<pair<string, ClientJobResults*> >::iterator endClient=data->allClientsResults.end();
    for (; itClient!=endClient; itClient++)
    {
        pair<string, ClientJobResults*> clientData = *itClient;

        AddClientData(clientData);

        int totalCode = JobStatus::NOT_EXECUTED;
        time_t totalDuration = 0;

        ClientJobResults::iterator itJob=clientData.second->begin();
        ClientJobResults::iterator endJob=clientData.second->end();
        for (; itJob!=endJob; itJob++)
        {
            pair<string, JobStatus*> jobData = *itJob;

            UpdateAttachments(jobData.second);
            totalCode = GetUpdatedCode(totalCode, jobData.second);
            totalDuration += jobData.second->GetDuration();

            AddJobData(jobData.first, jobData.second);
        }

        AddSummaryData(totalCode, totalDuration);
    }

    AddConfigurationErrorsData(configErrors);
    AddProgramData(version);
}

void AbstractStructuredReportCreator::UpdateAttachments(JobStatus *status)
{
    status->GetExternalFilenames(externalFiles);
    status->GetFileBuffers(fileBuffers);
}

int AbstractStructuredReportCreator::GetUpdatedCode(const int inputCode, JobStatus *status)
{
    return (status->IsWorseThan(inputCode)) ? status->GetCode() : inputCode;
}
