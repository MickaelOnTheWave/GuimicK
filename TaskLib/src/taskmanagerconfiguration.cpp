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

void TaskManagerConfiguration::ChangeConfigurationType(const ConfigurationType& type)
{
   delete typeConfiguration;
   typeConfiguration = CreateConfigurationManager(type);
}

void TaskManagerConfiguration::CreateTypeConfiguration()
{
   const ConfigurationType type = FindConfigurationType();
   ChangeConfigurationType(type);
}

ConfigurationType TaskManagerConfiguration::FindConfigurationType()
{
   if (HasOnlyJobList())
      return ClientConfigurationType;
   else if (parser.anonymousObject)
      return GetTypeByConfiguration();
   else // How would I know?
      return StandaloneConfigurationType;
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
      return ClientConfigurationType;
   else if (configurationTypeString == "Server")
      return ServerConfigurationType;
   else //if (configurationTypeString == "Standalone")
      return StandaloneConfigurationType;
}

AbstractTypeConfiguration* TaskManagerConfiguration::CreateConfigurationManager(
      const ConfigurationType& type
      )
{
   switch (type)
   {
      case ClientConfigurationType :
         return new ClientJobsConfiguration();
      case ServerConfigurationType :
         return new ServerConfiguration();
      default :
         return new StandaloneConfiguration();
   }
}
