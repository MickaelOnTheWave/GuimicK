#ifndef WAKEJOBCONFIGURATION_H
#define WAKEJOBCONFIGURATION_H

#include "abstractjobconfiguration.h"

class WakeJobConfiguration : public AbstractJobConfiguration
{
public:
    WakeJobConfiguration();

protected:
    virtual AbstractJob* CreateConfiguredJobAfterCheck(
                                             ConfigurationObject *confObject,
                                             std::vector<std::string> &errorMessages);
};

#endif // WAKEJOBCONFIGURATION_H
