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

private:
    bool WriteAttachments(AbstractReportCreator* reportCreator);
};

#endif // FILEREPORTDISPATCHER_H
