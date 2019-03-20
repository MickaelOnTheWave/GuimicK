#ifndef MODEL_H
#define MODEL_H

#include <QStringList>
#include <string>
#include <vector>

#include "taskmanagerconfiguration.h"
#include "SelfIdentity.h"
#include "standaloneconfiguration.h"

class TooledConfiguration
{
public:
   TooledConfiguration();
   TooledConfiguration(const TooledConfiguration& other);
   virtual ~TooledConfiguration();

   ConfigurationType GetConfigurationType() const;
   void SetConfigurationType(const ConfigurationType& type);

   bool LoadConfiguration(const std::wstring& file,
                          std::vector<std::wstring>& errors);
   bool SaveConfiguration(const std::wstring& file);

   std::list<AbstractJob*> GetJobList();
   void SetJobs(const std::vector<AbstractJob*>& jobs);

   void SetDefaultServerOptions();

   void ClearJobs();

   Agent* GetAgent();
   Client* GetClient();

   // TODO : remove this GetTmpConfig()
   StandaloneConfiguration* GetTmpConfiguration();

private:
   Agent* CreateDefaultAgent();

   TaskManagerConfiguration* configuration;
};

#endif // MODEL_H
