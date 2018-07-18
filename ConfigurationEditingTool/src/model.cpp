#include "model.h"

#include <list>

using namespace std;

Model::Model()
{
}

bool Model::LoadConfiguration(const std::string& file, std::vector<std::string>& errors)
{
   return configuration.LoadFromFile(file, errors);
}

void Model::SaveConfiguration(const std::string& file)
{
   configuration.SaveToFile(file);
}

list<AbstractJob*> Model::GetJobList()
{
   list<AbstractJob*> rawJobList;
   configuration.GetJobList(rawJobList);
   return rawJobList;
}

void Model::SetJobs(const std::vector<AbstractJob*>& jobs)
{
   configuration.SetJobList(jobs);
}

void Model::ClearJobs()
{
   configuration.ClearJobs();
}
