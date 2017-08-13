#include "clamavjobconfiguration.h"
#include "clamavjob.h"

ClamAvJobConfiguration::ClamAvJobConfiguration() : AbstractJobConfiguration("ClamAv")
{
}

AbstractJob *ClamAvJobConfiguration::CreateJob()
{
    return new ClamAvJob();
}

