#include "model.h"

#include <list>
#include "serverconfiguration.h"

using namespace std;

Model::Model() : configuration(NULL)
{
}

Model::~Model()
{
   delete configuration;
}

void Model::SetConfigurationManager(TaskManagerConfiguration* _configuration)
{
   configuration = _configuration;
}

bool Model::LoadConfiguration(const std::string& file, std::vector<std::string>& errors)
{
   return configuration->LoadFromFile(file, errors);
}

void Model::SaveConfiguration(const std::string& file)
{
   configuration->SaveToFile(file);
}

list<AbstractJob*> Model::GetJobList()
{
   list<AbstractJob*> rawJobList;
   configuration->GetJobList(rawJobList);
   return rawJobList;
}

void Model::SetJobs(const std::vector<AbstractJob*>& jobs)
{
   configuration->SetJobList(jobs);
}

void Model::SetDefaultServerOptions()
{
   ServerConfiguration* serverConfiguration = dynamic_cast<ServerConfiguration*>(configuration);
   if (serverConfiguration)
      serverConfiguration->SetAgent(CreateDefaultAgent());
}

void Model::ClearJobs()
{
   configuration->ClearJobs();
}

SelfIdentity* Model::CreateDefaultAgent()
{
   SelfIdentity* agent = new SelfIdentity();
   agent->name = "TaskManager Agent";
   return agent;
}
