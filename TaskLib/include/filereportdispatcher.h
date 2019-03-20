#ifndef FILEREPORTDISPATCHER_H
#define FILEREPORTDISPATCHER_H

#include "AbstractReportDispatcher.h"

class FileReportDispatcher : public AbstractReportDispatcher
{
public:
    FileReportDispatcher();

    virtual std::wstring GetName(void) const;
    virtual void Initialize(const AbstractTypeConfiguration* configuration);
    virtual bool Dispatch(AbstractReportCreator* reportCreator);

    void SetFolderName(const std::wstring& value);
    void SetFileName(const std::wstring& value);

private:
    bool WriteAttachments(AbstractReportCreator* reportCreator);

    std::wstring reportFolder;
    std::wstring reportFile;
};

#endif // FILEREPORTDISPATCHER_H
