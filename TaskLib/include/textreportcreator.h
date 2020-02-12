#ifndef CONSOLEREPORT_H
#define CONSOLEREPORT_H

#include <ctime>
#include <string>

#include "abstractstructuredreportcreator.h"
#include "jobstatus.h"

class TextReportCreator : public AbstractStructuredReportCreator
{
public:
	TextReportCreator();
   TextReportCreator(const TextReportCreator& other);
   virtual ~TextReportCreator();

   virtual AbstractReportCreator* Copy() const;

   virtual void UpdateWithDispatchError(const std::wstring& failedDispatcher,
                                        const std::wstring& dispatchError,
                                        const std::wstring& fallbackDispatcher);

protected:
   virtual void AddHeader(void);
   virtual void AddClientHeaderData(const std::pair<std::wstring, ClientJobResults *> &clientData);
   virtual void AddJobData(const std::wstring& jobName, JobStatus* status);
   virtual void AddSummaryData(const int code, const time_t duration);
   virtual void AddConfigurationErrorsData(const std::vector<std::wstring>& errors);
   virtual void AddProgramData(const std::wstring& version);

   void UpdateNameCellSize(ClientJobResults* data);

   unsigned int nameCellSize;
};

#endif // CONSOLEREPORT_H
