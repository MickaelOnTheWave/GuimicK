#ifndef CLAMAVJOBCONFIGURATION_H
#define CLAMAVJOBCONFIGURATION_H

#include "abstractjobconfiguration.h"

class ClamAvJobConfiguration : public AbstractJobConfiguration
{
public:
    ClamAvJobConfiguration();

protected:
    AbstractJob* CreateConfiguredJobAfterCheck(
            ConfigurationObject *confObject,
            std::vector<std::string> &errorMessages);
};

#endif // CLAMAVJOBCONFIGURATION_H
