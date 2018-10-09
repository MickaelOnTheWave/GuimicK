#include "taskmanagerconfiguration.h"

#include "clientjobsconfiguration.h"
#include "configurationparser.h"
#include "profiledjob.h"
#include "serverconfiguration.h"
#include "standaloneconfiguration.h"

using namespace std;

TaskManagerConfiguration::TaskManagerConfiguration()
    : typeConfiguration(NULL)
{
}

TaskManagerConfiguration::~TaskManagerConfiguration()
{
   delete typeConfiguration;
}

bool TaskManagerConfiguration::LoadFromFile(const string &fileName, vector<string> &errorMessages)
{
    errorMessages.clear();
    parser.ResetData();
    bool result = parser.ParseFile(fileName, errorMessages);
    if (!result)
        return false;

    CreateTypeConfiguration();
    return typeConfiguration->Load(parser, errorMessages);
}


bool TaskManagerConfiguration::SaveToFile(const string& filename)
{
   ofstream filestream(filename.c_str());
   if (!filestream.is_open())
      return false;

   typeConfiguration->SaveToOpenedFile(filestream);

   filestream.close();
   return true;
}

AbstractTypeConfiguration* TaskManagerConfiguration::GetTypeConfiguration()
{
   return typeConfiguration;
}

void TaskManagerConfiguration::CreateTypeConfiguration()
{
   const ConfigurationType type = FindConfigurationType();
   delete typeConfiguration;
   typeConfiguration = CreateConfigurationManager(type);
}

ConfigurationType TaskManagerConfiguration::FindConfigurationType()
{
   if (HasOnlyJobList())
      return ConfigurationType::Client;
   else if (parser.anonymousObject)
      return GetTypeByConfiguration();
   else // How would I know?
      return ConfigurationType::Standalone;
}

bool TaskManagerConfiguration::HasOnlyJobList() const
{
   return (parser.objectList.size() == 1 &&
           parser.objectList.front()->GetName() == "JobList");
}

ConfigurationType TaskManagerConfiguration::GetTypeByConfiguration() const
{
   const string configurationTypeString = parser.anonymousObject->GetProperty("Type");
   if (configurationTypeString == "Client")
      return ConfigurationType::Client;
   else if (configurationTypeString == "Server")
      return ConfigurationType::Server;
   else //if (configurationTypeString == "Standalone")
      return ConfigurationType::Standalone;
}

AbstractTypeConfiguration* TaskManagerConfiguration::CreateConfigurationManager(
      const ConfigurationType& type
      )
{
   switch (type)
   {
      case ConfigurationType::Client :
         return new ClientJobsConfiguration();
      case ConfigurationType::Server :
         return new ServerConfiguration();
      default :
         return new StandaloneConfiguration();
   }
}
