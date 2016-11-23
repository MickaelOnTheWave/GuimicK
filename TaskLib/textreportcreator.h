#ifndef CONSOLEREPORT_H
#define CONSOLEREPORT_H

#include <ctime>
#include <sstream>
#include <string>

#include "abstractreportcreator.h"
#include "jobstatus.h"

class TextReportCreator : public AbstractReportCreator
{
public:
	TextReportCreator();

	virtual std::string Generate(WorkResultData* data, const std::string& versionString);

protected:
	virtual void AddClientInformation(const std::string& clientName);
	virtual void AddJobInformation(const std::string& jobName, JobStatus* jobStatus, unsigned int nameCellSize);
	virtual void FinishReportGeneration(unsigned int nameCellSize);

	unsigned int FindBiggestJobNameSize(ClientJobResults* data);
	std::string SpacingString(unsigned int spacesToFill);

	std::stringstream report;
	std::string version;
	int generalCode;
	std::time_t generalDuration;
};

#endif // CONSOLEREPORT_H
