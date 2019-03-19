#ifndef TASKMANAGERCONFIGURATION_H
#define TASKMANAGERCONFIGURATION_H

#include <fstream>
#include <list>

#include "abstractjob.h"
#include "abstractjobconfiguration.h"
#include "abstracttypeconfiguration.h"
#include "clientworkmanager.h"
#include "configurationparser.h"
#include "configurationtype.h"

class ConfigurationObject;

class TaskManagerConfiguration
{
public:
   TaskManagerConfiguration();
   TaskManagerConfiguration(const TaskManagerConfiguration& other);
   virtual ~TaskManagerConfiguration();

   bool LoadFromFile(const std::string& fileName,
                     std::vector<std::string> &errorMessages);
   bool SaveToFile(const std::wstring& filename);

   AbstractTypeConfiguration* GetTypeConfiguration();

   void ChangeConfigurationType(const ConfigurationType& type);

private:
   void CreateTypeConfiguration();

   ConfigurationType FindConfigurationType();
   bool HasOnlyJobList() const;
   ConfigurationType GetTypeByConfiguration() const;

   AbstractTypeConfiguration* CreateConfigurationManager(
       const ConfigurationType& type
       );

   ConfigurationParser parser;
   AbstractTypeConfiguration* typeConfiguration;
};

#endif // TASKMANAGERCONFIGURATION_H
