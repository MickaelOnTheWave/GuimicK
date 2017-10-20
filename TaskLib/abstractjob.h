#ifndef ABSTRACTJOB_H
#define ABSTRACTJOB_H

#include <string>

#include "client.h"
#include "jobdebuginformationmanager.h"
#include "jobstatus.h"

class AbstractJob
{
public:
   AbstractJob(const std::string& _title);
   AbstractJob(const AbstractJob& other);
   virtual ~AbstractJob();

   virtual std::string GetName();
   void SetName(const std::string& value);

   std::string GetAttachmentName();

   virtual AbstractJob* Clone() = 0;

   /**
   Performs initial setup and gets needed data from client if
   needed.
   @return true on sucess, false on failure.
   @note failure is considered fatal, the job should not be ran if
   it wasn't sucessfully initialized.
   */
   virtual bool InitializeFromClient(Client* client);

   virtual bool IsInitialized(void) = 0;

   /**
   @note It is caller's responsibility to delete the JobStatus object.
   */
   virtual JobStatus* Run() = 0;

   virtual int GetOutputDebugInformationValue() const;
   virtual void SetOutputDebugInformation(const int value);

   void SetParentDebugManager(JobDebugInformationManager* manager);

protected:
    JobDebugInformationManager* debugManager;
    bool isDebugManagerParent;
    std::string title;
};

#endif // ABSTRACTJOB_H
