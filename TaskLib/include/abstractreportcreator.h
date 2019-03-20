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
   void Generate(WorkResultData* data, const std::wstring& version);

   virtual void Generate(WorkResultData* data,
                       const std::vector<std::wstring>& configErrors,
                       const std::wstring& version) = 0;
   virtual void UpdateWithDispatchError(const std::wstring& failedDispatcher,
                                      const std::wstring& fallbackDispatcher) = 0;

   std::wstring GetReportContent(void) const;
   void GetAssociatedFiles(std::vector<std::wstring>& _externalFiles,
                         std::vector<std::pair<std::wstring,std::wstring> >& _fileBuffers);
   bool HasAttachments() const;
protected:
   std::vector<std::wstring> externalFiles;
   std::vector<std::pair<std::wstring,std::wstring> > fileBuffers;

   std::wstring fullReport;
   std::wstringstream reportCore;
   std::wstringstream dispatchErrors;
   std::wstringstream programVersion;

   bool useProfiling;
};

#endif // ABSTRACTREPORT_H
