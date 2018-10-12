#ifndef STANDALONECONFIGURATION_H
#define STANDALONECONFIGURATION_H

#include "abstracttypeconfiguration.h"

#include "AbstractReportDispatcher.h"
#include "client.h"
#include "clientworkmanager.h"
#include "SelfIdentity.h"

class StandaloneConfiguration : public AbstractTypeConfiguration
{
public:
   static std::string MsgMissingClient;
   static std::string MsgMissingAgent;
   static std::string MsgOneClientSupported;

   StandaloneConfiguration();
   virtual ~StandaloneConfiguration();

   virtual void SaveToOpenedFile(std::ofstream& fileStream);

   virtual void GetJobList(std::list<AbstractJob*>& _jobList);

   virtual void SetJobList(const std::vector<AbstractJob*>& _jobList);

   virtual void ClearJobList();

   virtual ClientWorkManager* BuildWorkList(const bool withProfiling) const;

   std::string GetMasterEmail() const;

   bool GetLocalShutdown() const;

   std::string GetReportDispatching() const;

   AbstractReportCreator* GetReportCreator(void) const;

   const SelfIdentity *GetAgent() const;
   void SetAgent(SelfIdentity *agent);

   Client* GetClient();

   AbstractReportDispatcher* CreateReportDispatcher(const bool commandLinePreventsEmail) const;

   bool IsReportHtml(void) const;

protected:
   virtual void CreateAgent(ConfigurationObject* confObject,
                            std::vector<std::string>& errorMessages);
   virtual bool CreateClient(ConfigurationObject* confObject,
                             std::vector<std::string> &errorMessages);

   Client* client;

private:
   virtual void FillRootObjects(const std::list<ConfigurationObject*>& objectList,
                              std::vector<std::string> &errorMessages);
   virtual void FillGlobalProperties(ConfigurationObject* object,
                                   std::vector<std::string> &errorMessages);
   virtual bool IsConfigurationConsistent(std::vector<std::string> &errorMessages);

   void SaveAgentToOpenedFile(std::ofstream& file);
   void SaveClientToOpenedFile(std::ofstream& file);
   void SaveReportOptionsToOpenedFile(std::ofstream& file);
   void SaveGlobalPropertiesToOpenedFile(std::ofstream& file);

   void CreateReport(ConfigurationObject* confObject,
                     std::vector<std::string>& errorMessages);

   AbstractReportCreator* CreateReportObject(const std::string& type) const;

   bool IsEmailDataComplete() const;

   void CreateDefaultClient();
   void CreateDefaultSelfIdentity();

   AbstractReportCreator* reportCreator;
   SelfIdentity* self;
   std::string reportType;
   std::string cssFile;
   std::string masterEmail;
   std::string reportDispatching;
   bool shutdown;
};

#endif // STANDALONECONFIGURATION_H
