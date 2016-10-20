#ifndef CONSOLEREPORT_H
#define CONSOLEREPORT_H

#include <ctime>
#include <list>
#include <sstream>
#include <string>

#include "AbstractReport.h"
#include "jobstatus.h"

class TextReportCreator : public AbstractReportCreator
{
public:
	TextReportCreator();

	virtual std::string Generate(WorkResultData* data, const std::string& versionString);
	virtual void GetAssociatedFiles(std::list<std::string> &fileList);

protected:
	virtual void AddClientInformation(const std::string& clientName);
	virtual void AddJobInformation(const std::string& jobName, JobStatus* jobStatus, unsigned int nameCellSize);
	virtual void FinishReportGeneration(unsigned int nameCellSize);

	unsigned int FindBiggestJobNameSize(ClientJobResults* data);
	std::string SpacingString(unsigned int spacesToFill);

	std::vector<std::string> jobsFilenames;
	std::stringstream report;
	std::string version;
	int generalCode;
	std::time_t generalDuration;
};

#endif // CONSOLEREPORT_H
