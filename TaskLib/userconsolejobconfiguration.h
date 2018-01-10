#ifndef USERCONSOLEJOBCONFIGURATION_H
#define USERCONSOLEJOBCONFIGURATION_H

#include "abstractjobdefaultconfiguration.h"

class UserConsoleJobConfiguration : public AbstractJobDefaultConfiguration
{
public:
    static const std::string CommandProperty;
    static const std::string ParameterProperty;
    static const std::string ReturnCodeProperty;
    static const std::string ExpectedOutputProperty;
    static const std::string OutputFilenameProperty;
    static const std::string ParserCommandProperty;
    static const std::string ParserUsesBufferProperty;
    static const std::string UserAttachmentObject;

    UserConsoleJobConfiguration();
    UserConsoleJobConfiguration(const std::string& tag);

    virtual bool IsRightJob(AbstractJob *job);
    virtual ConfigurationObject* CreateConfigurationObject(AbstractJob* job);

protected:
    virtual AbstractJob* CreateJob();
    virtual void ConfigureJob(AbstractJob* job,
                              ConfigurationObject *confObject,
                              std::vector<std::string> &errorMessages);
    virtual void FillKnownProperties(std::vector<std::string>& properties);
    virtual void FillKnownSubObjects(std::vector<std::string>& objects);
};

#endif // USERCONSOLEJOBCONFIGURATION_H
