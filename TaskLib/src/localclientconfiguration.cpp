#include "localclientconfiguration.h"

#include "clientjobsconfiguration.h"

using namespace std;

wstring LocalClientConfiguration::MsgClientWithoutName = L"Error : Client without name";
wstring LocalClientConfiguration::MsgClientWithoutJobs = L"Warning : Client has no jobs";
wstring LocalClientConfiguration::MsgDeprecatedRemoteOption = L"Warning : Remote option deprecated";


Client* LocalClientConfiguration::CreateConfiguredClient(ConfigurationObject* confObject,
                                                    vector<wstring>& errorMessages)
{
	client = NULL;

	const wstring deprecatedRemoteJobOption = confObject->GetProperty(L"remoteJobList");
	if (deprecatedRemoteJobOption != L"")
		 errorMessages.push_back(MsgDeprecatedRemoteOption);

	client = new Client();
	client->SetName(confObject->GetProperty(L"Name"));
	SetAllClientProperties(confObject);

	bool ok = AreClientPropertiesConsistent(confObject, errorMessages);
	if (ok)
	{
		const wstring debugOptionwstring = confObject->GetProperty(L"showDebugInformation");
		const int debugOption = (debugOptionwstring != L"") ?
		                           DebugOutput::GetValue(debugOptionwstring) :
		                           DebugOutput::UNDEFINED;
		ClientJobsConfiguration jobsConfiguration(debugOption);
		ConfigurationObject* jobListObject = confObject->GetObject(L"JobList");
		if (jobListObject)
			ok = jobsConfiguration.LoadFromConfigurationObject(jobListObject, errorMessages);

		if (ok)
		{
			list<AbstractJob*> jobList;
			jobsConfiguration.GetJobList(jobList);
			if (jobList.empty())
				 errorMessages.push_back(MsgClientWithoutJobs);
			else
				client->SetJobList(jobList);
		}
	}

	if (ok == false)
	{
		delete client;
		client = NULL;
	}

	return client;
}

void LocalClientConfiguration::SetAllClientProperties(ConfigurationObject* object)
{
	map<wstring, wstring>::iterator itProp = object->propertyList.begin();
	map<wstring, wstring>::iterator endProp = object->propertyList.end();
	for (; itProp != endProp; itProp++)
	{
		 pair<wstring, wstring> currentProp = *itProp;

		 // Name already handled
		 if (currentProp.first == L"Name")
			  continue;

		 client->AddProperty(currentProp.first, currentProp.second);
	}
}

bool LocalClientConfiguration::AreClientPropertiesConsistent(
   ConfigurationObject*, std::vector<wstring> &errorMessages)
{
	if (client->GetName() == L"")
	{
		errorMessages.push_back(MsgClientWithoutName);
		return false;
	}
	else
		return true;
}
