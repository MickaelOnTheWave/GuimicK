#include "taskmanagerconfiguration.h"

#include "clientjobsconfiguration.h"
#include "configurationparser.h"
#include "profiledjob.h"
#include "serverconfiguration.h"
#include "standaloneconfiguration.h"
#include "stringtools.h"

using namespace std;

TaskManagerConfiguration::TaskManagerConfiguration()
    : typeConfiguration(NULL)
{
}

TaskManagerConfiguration::TaskManagerConfiguration(const TaskManagerConfiguration& other)
{
   typeConfiguration = (other.typeConfiguration) ? other.typeConfiguration->Copy() : NULL;
}

TaskManagerConfiguration::~TaskManagerConfiguration()
{
   delete typeConfiguration;
}

bool TaskManagerConfiguration::LoadFromFile(const wstring &fileName, vector<wstring> &errorMessages)
{
    errorMessages.clear();
    parser.ResetData();
    bool result = parser.ParseFile(fileName, errorMessages);
    if (!result)
        return false;

    CreateTypeConfiguration();
    return typeConfiguration->Load(parser, errorMessages);
}


bool TaskManagerConfiguration::SaveToFile(const wstring& filename)
{
#ifdef _MSC_VER
   wofstream filestream(filename.c_str());
#else
   const string utf8Filename = StringTools::UnicodeToUtf8(filename);
   wofstream filestream(utf8Filename.c_str());
#endif
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
           parser.objectList.front()->GetName() == L"JobList");
}

ConfigurationType TaskManagerConfiguration::GetTypeByConfiguration() const
{
   const wstring configurationTypewstring = parser.anonymousObject->GetProperty(L"Type");
   if (configurationTypewstring == L"Client")
      return ClientConfigurationType;
   else if (configurationTypewstring == L"Server")
      return ServerConfigurationType;
   else //if (configurationTypewstring == "Standalone")
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
