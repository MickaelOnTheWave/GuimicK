#ifndef MODEL_H
#define MODEL_H

#include <QStringList>
#include <string>
#include <vector>

#include "clientjobsconfiguration.h"

class Model
{
public:
   Model();

   bool LoadConfiguration(const std::string& file,
                          std::vector<std::string>& errors);
   void SaveConfiguration(const std::string& file);

   std::list<AbstractJob*> GetJobList();

private:
   ClientJobsConfiguration configuration;

};

#endif // MODEL_H
