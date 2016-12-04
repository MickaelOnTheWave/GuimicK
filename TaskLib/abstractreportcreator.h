#ifndef ABSTRACTREPORT_H
#define ABSTRACTREPORT_H

#include "workresultdata.h"
#include <vector>

// TODO : improve class design. As it is, the Generate() method both generates data and returns partial report data.
// For the sake of clarity, it should only generate, a separate method should return main report data. There is already
// one for attachments.
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
