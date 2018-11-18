#ifndef FILEREPORTDISPATCHER_H
#define FILEREPORTDISPATCHER_H

#include "AbstractReportDispatcher.h"

class FileReportDispatcher : public AbstractReportDispatcher
{
public:
    FileReportDispatcher();

    virtual std::string GetName(void) const;
    virtual void Initialize(const AbstractTypeConfiguration* configuration);
    virtual bool Dispatch(AbstractReportCreator* reportCreator);

    void SetFolderName(const std::string& value);
    void SetFileName(const std::string& value);

private:
    bool WriteAttachments(AbstractReportCreator* reportCreator);

    std::string reportFolder;
    std::string reportFile;
};

#endif // FILEREPORTDISPATCHER_H
