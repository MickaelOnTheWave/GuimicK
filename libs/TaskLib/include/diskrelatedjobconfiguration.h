#ifndef DISKRELATEDJOBCONFIGURATION_H
#define DISKRELATEDJOBCONFIGURATION_H

#include "abstractjobdefaultconfiguration.h"

class DiskRelatedJobConfiguration : public AbstractJobDefaultConfiguration
{
public:
    static const std::wstring DriveProperty;

    DiskRelatedJobConfiguration(const std::wstring &tag);

    virtual bool IsRightJob(AbstractJob *job);

    virtual ConfigurationObject* CreateConfigurationObject(AbstractJob* job);

protected:
    virtual void ConfigureJob(AbstractJob* job,
                              ConfigurationObject *confObject,
                              std::vector<std::wstring> &errorMessages);
    virtual void FillKnownProperties(std::vector<std::wstring>& properties);
};

#endif // DISKRELATEDJOBCONFIGURATION_H
