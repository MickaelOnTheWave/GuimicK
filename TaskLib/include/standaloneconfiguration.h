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
   StandaloneConfiguration(const StandaloneConfiguration& other);
   virtual ~StandaloneConfiguration();

   virtual ConfigurationType GetType() const;

   virtual AbstractTypeConfiguration* Copy() const;

   virtual void SaveToOpenedFile(std::ofstream& fileStream);

   virtual void GetJobList(std::list<AbstractJob*>& _jobList);

   virtual void SetJobList(const std::vector<AbstractJob*>& _jobList);

   virtual void ClearJobList();

   virtual ClientWorkManager* BuildWorkList(const bool withProfiling) const;

   std::string GetMasterEmail() const;
   void SetMasterEmail(const std::string& value);

   bool GetLocalShutdown() const;
   void SetLocalShutdown(const bool value);

   std::string GetReportDispatching() const;
   void SetReportDispatching(const std::string& value);

   std::string GetReportType() const;
   void SetReportType(const std::string& value);
   AbstractReportCreator* GetReportCreator(void) const;

   std::string GetReportCss() const;
   void SetReportCss(const std::string& value);


   Agent* GetAgent() const;
   void SetAgent(Agent *_agent);

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

   void CheckReportDispatchingErrors(std::vector<std::string> &errorMessages);

   void SaveClientToOpenedFile(std::ofstream& file);
   void SaveReportOptionsToOpenedFile(std::ofstream& file);
   void SaveGlobalPropertiesToOpenedFile(std::ofstream& file);

   void CreateReport(ConfigurationObject* confObject,
                     std::vector<std::string>& errorMessages);

   void ChangeReportCreator();

   bool IsEmailDataComplete() const;
   bool IsFileDataComplete() const;

   AbstractReportCreator* reportCreator;
   Agent* agent;
   std::string reportType;
   std::string cssFile;
   std::string masterEmail;
   std::string reportDispatching;
   bool shutdown;
};

#endif // STANDALONECONFIGURATION_H
