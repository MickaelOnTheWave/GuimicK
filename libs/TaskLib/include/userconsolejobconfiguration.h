#ifndef USERCONSOLEJOBCONFIGURATION_H
#define USERCONSOLEJOBCONFIGURATION_H

#include "abstractjobdefaultconfiguration.h"
#include "userconsolejob.h"

class UserConsoleJobConfiguration : public AbstractJobDefaultConfiguration
{
public:
    static const std::wstring CommandProperty;
    static const std::wstring ParameterProperty;
    static const std::wstring ReturnCodeProperty;
    static const std::wstring ExpectedOutputProperty;
    static const std::wstring OutputFilenameProperty;
    static const std::wstring ParserCommandProperty;
    static const std::wstring ParserUsesBufferProperty;
    static const std::wstring UserAttachmentObject;

    UserConsoleJobConfiguration();
    UserConsoleJobConfiguration(const std::wstring& tag);

    virtual bool IsRightJob(AbstractJob *job);
    virtual ConfigurationObject* CreateConfigurationObject(AbstractJob* job);

protected:
    virtual AbstractJob* CreateJob();
    virtual void ConfigureJob(AbstractJob* job,
                              ConfigurationObject *confObject,
                              std::vector<std::wstring> &errorMessages);
    virtual void FillKnownProperties(std::vector<std::wstring>& properties);
    virtual void FillKnownSubObjects(std::vector<std::wstring>& objects);

    void ConfigureObjectFromJob(ConfigurationObject* confObject,
                                UserConsoleJob* job);
};

#endif // USERCONSOLEJOBCONFIGURATION_H
