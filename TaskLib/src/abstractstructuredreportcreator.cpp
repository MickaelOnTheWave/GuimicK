#include "abstractstructuredreportcreator.h"

#include "resultcollectionstatus.h"

using namespace std;

AbstractStructuredReportCreator::AbstractStructuredReportCreator()
   : globalCode(JobStatus::NOT_EXECUTED), globalDuration(0)
{
}

AbstractStructuredReportCreator::~AbstractStructuredReportCreator()
{
}

void AbstractStructuredReportCreator::Generate(WorkResultData *data,
                                               const vector<string> &configErrors,
                                               const string &version)
{
    fullReport = "";
    reportCore.str("");
    dispatchErrors.str("");
    programVersion.str("");

    AddHeader();

    vector<ClientResult>::iterator itClient=data->allClientsResults.begin();
    vector<ClientResult>::iterator endClient=data->allClientsResults.end();
    for (; itClient!=endClient; itClient++)
    {
        pair<string, ClientJobResults*> clientData = *itClient;

        AddClientHeaderData(clientData);

        globalCode = JobStatus::NOT_EXECUTED;
        globalDuration = 0;

        AddJobResultData(clientData);
        AddSummaryData(globalCode, globalDuration);
    }

    AddConfigurationErrorsData(configErrors);
    AddProgramData(version);

    fullReport = reportCore.str() + programVersion.str();
}

void AbstractStructuredReportCreator::AddJobResultData(const ClientResult& clientData)
{
   ClientJobResults::iterator itJob=clientData.second->begin();
   ClientJobResults::iterator endJob=clientData.second->end();
   for (; itJob!=endJob; itJob++)
   {
      pair<string, JobStatus*> jobData = *itJob;

      ResultCollectionStatus* collectionStatus = dynamic_cast<ResultCollectionStatus*>(jobData.second);
      if (collectionStatus)
      {
         WorkResultData* remoteResults = collectionStatus->GetResults();
         AddJobResultData(remoteResults->allClientsResults.front());
      }
      else
      {
         UpdateAttachments(jobData.second);
         globalCode = GetUpdatedCode(globalCode, jobData.second);
         globalDuration += jobData.second->GetDuration();

         AddJobData(jobData.first, jobData.second);
      }
   }
}

void AbstractStructuredReportCreator::UpdateAttachments(JobStatus *status)
{
    status->GetExternalFilenames(externalFiles);
    status->GetFileBuffers(fileBuffers);
}

int AbstractStructuredReportCreator::GetUpdatedCode(const int inputCode, JobStatus *status) const
{
    return (status->IsWorseThan(inputCode)) ? status->GetCode() : inputCode;
}
