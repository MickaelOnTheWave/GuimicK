#ifndef MODEL_H
#define MODEL_H

#include <QStringList>
#include <string>
#include <vector>

#include "taskmanagerconfiguration.h"
#include "SelfIdentity.h"

class Model
{
public:
   Model();
   virtual ~Model();

   ConfigurationType GetConfigurationType() const;
   void SetConfigurationType(const ConfigurationType& type);

   bool LoadConfiguration(const std::string& file,
                          std::vector<std::string>& errors);
   void SaveConfiguration(const std::string& file);

   std::list<AbstractJob*> GetJobList();
   void SetJobs(const std::vector<AbstractJob*>& jobs);

   void SetDefaultServerOptions();

   void ClearJobs();

   SelfIdentity* GetAgent();
   Client* GetClient();

private:
   SelfIdentity* CreateDefaultAgent();

   TaskManagerConfiguration* configuration;

};

#endif // MODEL_H
