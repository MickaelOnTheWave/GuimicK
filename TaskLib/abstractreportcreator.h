#ifndef ABSTRACTREPORT_H
#define ABSTRACTREPORT_H

#include "workresultdata.h"
#include <vector>

class AbstractReportCreator
{
public:
	virtual ~AbstractReportCreator() {}

	virtual std::string Generate(WorkResultData* data, const std::string& version) = 0;
    void GetAssociatedFiles(std::vector<std::string>& _externalFiles,
                            std::vector<std::pair<std::string,std::string> >& _fileBuffers);

protected:
    std::vector<std::string> externalFiles;
    std::vector<std::pair<std::string,std::string> > fileBuffers;

};

#endif // ABSTRACTREPORT_H
