#include "localclientconfiguration.h"

#include "clientjobsconfiguration.h"

using namespace std;

string LocalClientConfiguration::MsgClientWithoutName = "Error : Client without name";
string LocalClientConfiguration::MsgClientWithoutJobs = "Warning : Client has no jobs";
string LocalClientConfiguration::MsgDeprecatedRemoteOption = "Warning : Remote option deprecated";


Client* LocalClientConfiguration::CreateConfiguredClient(ConfigurationObject* confObject,
                                                    vector<string>& errorMessages)
{
	client = NULL;

	const string deprecatedRemoteJobOption = confObject->GetProperty("remoteJobList");
	if (deprecatedRemoteJobOption != "")
		 errorMessages.push_back(MsgDeprecatedRemoteOption);

	client = new Client();
	client->SetName(confObject->GetProperty("Name"));
	SetAllClientProperties(confObject);

	bool ok = AreClientPropertiesConsistent(confObject, errorMessages);
	if (ok)
	{
		const string debugOptionString = confObject->GetProperty("showDebugInformation");
		const int debugOption = (debugOptionString != "") ?
		                           DebugOutput::GetValue(debugOptionString) :
		                           DebugOutput::UNDEFINED;
		ClientJobsConfiguration jobsConfiguration(debugOption);
		ConfigurationObject* jobListObject = confObject->GetObject("JobList");
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
	map<string, string>::iterator itProp = object->propertyList.begin();
	map<string, string>::iterator endProp = object->propertyList.end();
	for (; itProp != endProp; itProp++)
	{
		 pair<string, string> currentProp = *itProp;

		 // Name already handled
		 if (currentProp.first == "Name")
			  continue;

		 client->AddProperty(currentProp.first, currentProp.second);
	}
}

bool LocalClientConfiguration::AreClientPropertiesConsistent(
      ConfigurationObject *object, std::vector<string> &errorMessages
      )
{
	if (client->GetName() == "")
	{
		errorMessages.push_back(MsgClientWithoutName);
		return false;
	}
	else
		return true;
}
