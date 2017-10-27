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

QStringList Model::GetJobList()
{
   list<AbstractJob*> rawJobList;
   configuration.GetJobList(rawJobList);

   QStringList jobListInformation;
   for (auto it : rawJobList)
      jobListInformation.push_back(it->GetName().c_str());
   return jobListInformation;
}
