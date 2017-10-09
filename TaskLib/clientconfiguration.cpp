#include "clientconfiguration.h"

#include "clientjobsconfiguration.h"

using namespace std;

string ClientConfiguration::MsgClientWithoutName = "Error : Client without name";
string ClientConfiguration::MsgClientWithoutJobs = "Warning : Client has no jobs";
string ClientConfiguration::MsgDeprecatedRemoteOption = "Warning : Remote option deprecated";


ClientConfiguration::ClientConfiguration()
{
}

Client* ClientConfiguration::CreateConfiguredClient(ConfigurationObject* confObject,
                                                    vector<string>& errorMessages)
{
   client = NULL;

   const string clientName = confObject->GetProperty("Name");
   if (clientName == "")
   {
       errorMessages.push_back(MsgClientWithoutName);
       return NULL;
   }

   const string deprecatedRemoteJobOption = confObject->GetProperty("remoteJobList");
   if (deprecatedRemoteJobOption != "")
       errorMessages.push_back(MsgDeprecatedRemoteOption);

   client = new Client();
   client->SetName(clientName);

   bool ok = AreClientPropertiesConsistent(confObject, errorMessages);
   if (ok)
   {
      ClientJobsConfiguration jobsConfiguration;
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

bool ClientConfiguration::AreClientPropertiesConsistent(ConfigurationObject *object,
                                                  std::vector<string> &errorMessages)
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

    if (client->GetProperty("ip") == "")
    {
        errorMessages.push_back("Error : Client without IP address");
        return false;
    }
    else if (client->GetProperty("sshuser") == "")
    {
        errorMessages.push_back("Error : Client without Ssh user");
        return false;
    }
    else
        return true;
}
