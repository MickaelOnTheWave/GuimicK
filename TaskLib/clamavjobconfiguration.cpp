#include "clamavjobconfiguration.h"
#include "clamavjob.h"

ClamAvJobConfiguration::ClamAvJobConfiguration() : AbstractJobDefaultConfiguration("ClamAv")
{
}

AbstractJob *ClamAvJobConfiguration::CreateJob()
{
    return new ClamAvJob();
}

