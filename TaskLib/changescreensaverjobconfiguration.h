#ifndef CHANGESCREENSAVERJOBCONFIGURATION_H
#define CHANGESCREENSAVERJOBCONFIGURATION_H

#include "abstractjobdefaultconfiguration.h"

class ChangeScreensaverJobConfiguration : public AbstractJobDefaultConfiguration
{
public:
    static const std::string TimeoutProperty;

    ChangeScreensaverJobConfiguration();

    virtual bool IsRightJob(AbstractJob* job);
    virtual ConfigurationObject* CreateConfigurationObject(AbstractJob *job);

protected:
    virtual AbstractJob* CreateJob();
    virtual void ConfigureJob(AbstractJob* job,
                              ConfigurationObject *confObject,
                              std::vector<std::string> &errorMessages);
    virtual void FillKnownProperties(std::vector<std::string>& properties);
};

#endif // CHANGESCREENSAVERJOBCONFIGURATION_H
