#ifndef ABSTRACTREPORT_H
#define ABSTRACTREPORT_H

#include "workresultdata.h"
#include <list>

class AbstractReportCreator
{
public:
	virtual ~AbstractReportCreator() {}

	virtual std::string Generate(WorkResultData* data, const std::string& version) = 0;
	virtual void GetAssociatedFiles(std::list<std::string>& fileList) = 0;
};

#endif // ABSTRACTREPORT_H
