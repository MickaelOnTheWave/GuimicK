#include "tooledconfiguration.h"

#include <list>
#include "serverconfiguration.h"

using namespace std;

TooledConfiguration::TooledConfiguration() : configuration(new TaskManagerConfiguration())
{
}

TooledConfiguration::~TooledConfiguration()
{
   delete configuration;
}

ConfigurationType TooledConfiguration::GetConfigurationType() const
{
   return configuration->GetTypeConfiguration()->GetType();
}

void TooledConfiguration::SetConfigurationType(const ConfigurationType& type)
{
   configuration->ChangeConfigurationType(type);
}

bool TooledConfiguration::LoadConfiguration(const std::string& file, std::vector<std::string>& errors)
{
   return configuration->LoadFromFile(file, errors);
}

void TooledConfiguration::SaveConfiguration(const std::string& file)
{
   configuration->SaveToFile(file);
}

list<AbstractJob*> TooledConfiguration::GetJobList()
{
   list<AbstractJob*> rawJobList;
   configuration->GetTypeConfiguration()->GetJobList(rawJobList);
   return rawJobList;
}

void TooledConfiguration::SetJobs(const std::vector<AbstractJob*>& jobs)
{
   configuration->GetTypeConfiguration()->SetJobList(jobs);
}

void TooledConfiguration::SetDefaultServerOptions()
{
   ServerConfiguration* serverConfiguration = dynamic_cast<ServerConfiguration*>(configuration);
   if (serverConfiguration)
      serverConfiguration->SetAgent(CreateDefaultAgent());
}

void TooledConfiguration::ClearJobs()
{
   configuration->GetTypeConfiguration()->ClearJobList();
}

SelfIdentity* TooledConfiguration::GetAgent()
{
   auto typedConf = dynamic_cast<StandaloneConfiguration*>(
                       configuration->GetTypeConfiguration()
                       );
   return (typedConf) ? typedConf->GetAgent() : nullptr;
}

Client* TooledConfiguration::GetClient()
{
   auto typedConf = dynamic_cast<StandaloneConfiguration*>(
                       configuration->GetTypeConfiguration()
                       );
   return (typedConf) ? typedConf->GetClient() : nullptr;
}

StandaloneConfiguration* TooledConfiguration::GetTmpConfiguration()
{
   auto typedConf = dynamic_cast<StandaloneConfiguration*>(
                       configuration->GetTypeConfiguration()
                       );
   return typedConf;
}

SelfIdentity* TooledConfiguration::CreateDefaultAgent()
{
   SelfIdentity* agent = new SelfIdentity();
   agent->name = "TaskManager Agent";
   return agent;
}
