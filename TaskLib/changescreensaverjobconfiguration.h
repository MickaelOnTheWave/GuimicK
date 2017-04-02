#ifndef CHANGESCREENSAVERJOBCONFIGURATION_H
#define CHANGESCREENSAVERJOBCONFIGURATION_H

#include "abstractjobconfiguration.h"

class ChangeScreensaverJobConfiguration : public AbstractJobConfiguration
{
public:
    ChangeScreensaverJobConfiguration();

protected:
    virtual AbstractJob* CreateConfiguredJobAfterCheck(
                                        ConfigurationObject *confObject,
                                        std::vector<std::string> &errorMessages);
    virtual void FillKnownProperties(std::vector<std::string>& properties);
};

#endif // CHANGESCREENSAVERJOBCONFIGURATION_H
