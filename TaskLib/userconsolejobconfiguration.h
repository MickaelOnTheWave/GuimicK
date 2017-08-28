#ifndef USERCONSOLEJOBCONFIGURATION_H
#define USERCONSOLEJOBCONFIGURATION_H

#include "abstractjobdefaultconfiguration.h"

class UserConsoleJobConfiguration : public AbstractJobDefaultConfiguration
{
public:
    static const std::string TitleProperty;
    static const std::string CommandProperty;
    static const std::string ReturnCodeProperty;
    static const std::string ExpectedOutputProperty;
    static const std::string OutputFilenameProperty;
    static const std::string ParserCommandProperty;
    static const std::string ParserUsesBufferProperty;

    UserConsoleJobConfiguration();
    UserConsoleJobConfiguration(const std::string& tag);

protected:
    virtual AbstractJob* CreateJob();
    virtual void ConfigureJob(AbstractJob* job,
                              ConfigurationObject *confObject,
                              std::vector<std::string> &errorMessages);
    void FillKnownProperties(std::vector<std::string>& properties);
};

#endif // USERCONSOLEJOBCONFIGURATION_H
