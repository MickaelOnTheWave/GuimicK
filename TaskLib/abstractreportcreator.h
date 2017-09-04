#ifndef ABSTRACTREPORT_H
#define ABSTRACTREPORT_H

#include "workresultdata.h"
#include <sstream>
#include <vector>

class AbstractReportCreator
{
public:
    AbstractReportCreator();
	virtual ~AbstractReportCreator() {}

    void UseProfileColumn(const bool value);
    void Generate(WorkResultData* data, const std::string& version);
    virtual void Generate(WorkResultData* data,
                          const std::vector<std::string>& configErrors,
                          const std::string& version) = 0;
    std::string GetReportContent(void) const;
    void GetAssociatedFiles(std::vector<std::string>& _externalFiles,
                            std::vector<std::pair<std::string,std::string> >& _fileBuffers);

protected:
    std::vector<std::string> externalFiles;
    std::vector<std::pair<std::string,std::string> > fileBuffers;
    std::stringstream report;
    bool useProfiling;
};

#endif // ABSTRACTREPORT_H
