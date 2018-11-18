#ifndef ABSTRACTREPORT_H
#define ABSTRACTREPORT_H

#include "workresultdata.h"
#include <sstream>
#include <vector>

class AbstractReportCreator
{
public:
   AbstractReportCreator();
   AbstractReportCreator(const AbstractReportCreator& other);
   virtual ~AbstractReportCreator() {}

   virtual AbstractReportCreator* Copy() const = 0;

   void UseProfileColumn(const bool value);
   void Generate(WorkResultData* data, const std::string& version);

   virtual void Generate(WorkResultData* data,
                       const std::vector<std::string>& configErrors,
                       const std::string& version) = 0;
   virtual void UpdateWithDispatchError(const std::string& failedDispatcher,
                                      const std::string& fallbackDispatcher) = 0;

   std::string GetReportContent(void) const;
   void GetAssociatedFiles(std::vector<std::string>& _externalFiles,
                         std::vector<std::pair<std::string,std::string> >& _fileBuffers);
   bool HasAttachments() const;
protected:
   std::vector<std::string> externalFiles;
   std::vector<std::pair<std::string,std::string> > fileBuffers;

   std::string fullReport;
   std::stringstream reportCore;
   std::stringstream dispatchErrors;
   std::stringstream programVersion;

   bool useProfiling;
};

#endif // ABSTRACTREPORT_H
