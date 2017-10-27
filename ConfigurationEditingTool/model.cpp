#include "model.h"

Model::Model()
{

}

bool Model::LoadConfiguration(const std::string& file, std::vector<std::string>& errors)
{
   errors.push_back("Error 1");
   errors.push_back("Error 2");

   return true;
}

void Model::SaveConfiguration(const std::string& file)
{
   // TODO implement
}
