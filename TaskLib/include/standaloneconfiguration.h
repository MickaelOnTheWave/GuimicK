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
   static std::wstring MsgMissingClient;
   static std::wstring MsgMissingAgent;
   static std::wstring MsgOneClientSupported;

   StandaloneConfiguration();
   StandaloneConfiguration(const StandaloneConfiguration& other);
   virtual ~StandaloneConfiguration();

   virtual ConfigurationType GetType() const;

   virtual AbstractTypeConfiguration* Copy() const;

   virtual void SaveToOpenedFile(std::wofstream& fileStream);

   virtual void GetJobList(std::list<AbstractJob*>& _jobList);

   virtual void SetJobList(const std::vector<AbstractJob*>& _jobList);

   virtual void ClearJobList();

   virtual ClientWorkManager* BuildWorkList(const bool withProfiling) const;

   std::wstring GetMasterEmail() const;
   void SetMasterEmail(const std::wstring& value);

   bool GetLocalShutdown() const;
   void SetLocalShutdown(const bool value);

   std::wstring GetReportDispatching() const;
   void SetReportDispatching(const std::wstring& value);

   std::wstring GetReportType() const;
   void SetReportType(const std::wstring& value);
   AbstractReportCreator* GetReportCreator(void) const;

   std::wstring GetReportCss() const;
   void SetReportCss(const std::wstring& value);


   Agent* GetAgent() const;
   void SetAgent(Agent *_agent);

   Client* GetClient();

   AbstractReportDispatcher* CreateReportDispatcher(const bool commandLinePreventsEmail) const;

   bool IsReportHtml(void) const;

protected:
   virtual void CreateAgent(ConfigurationObject* confObject,
                            std::vector<std::wstring>& errorMessages);
   virtual bool CreateClient(ConfigurationObject* confObject,
                             std::vector<std::wstring> &errorMessages);

   Client* client;

private:
   virtual void FillRootObjects(const std::list<ConfigurationObject*>& objectList,
                              std::vector<std::wstring> &errorMessages);
   virtual void FillGlobalProperties(ConfigurationObject* object,
                                   std::vector<std::wstring> &errorMessages);
   virtual bool IsConfigurationConsistent(std::vector<std::wstring> &errorMessages);

   void CheckReportDispatchingErrors(std::vector<std::wstring> &errorMessages);

   void SaveClientToOpenedFile(std::wofstream& file);
   void SaveReportOptionsToOpenedFile(std::wofstream& file);
   void SaveGlobalPropertiesToOpenedFile(std::wofstream& file);

   void SaveClientPropertiesToOpenedFile(std::wofstream& file);

   void CreateReport(ConfigurationObject* confObject,
                     std::vector<std::wstring>& errorMessages);

   void ChangeReportCreator();

   bool IsEmailDataComplete() const;
   bool IsFileDataComplete() const;

   AbstractReportCreator* reportCreator;
   Agent* agent;
   std::wstring reportType;
   std::wstring cssFile;
   std::wstring masterEmail;
   std::wstring reportDispatching;
   bool shutdown;
};

#endif // STANDALONECONFIGURATION_H
