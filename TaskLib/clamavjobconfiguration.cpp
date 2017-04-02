#include "clamavjobconfiguration.h"
#include "clamavjob.h"

ClamAvJobConfiguration::ClamAvJobConfiguration() : AbstractJobConfiguration("ClamAv")
{
}

AbstractJob *ClamAvJobConfiguration::CreateConfiguredJobAfterCheck(
        ConfigurationObject *,
        std::vector<std::string> &)
{
    return new ClamAvJob();
}
