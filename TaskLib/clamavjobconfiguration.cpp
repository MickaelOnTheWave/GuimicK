#include "clamavjobconfiguration.h"
#include "clamavjob.h"

ClamAvJobConfiguration::ClamAvJobConfiguration() : AbstractJobDefaultConfiguration("ClamAv")
{
}

bool ClamAvJobConfiguration::IsRightJob(AbstractJob* job)
{
   return (dynamic_cast<ClamAvJob*>(job) != NULL);
}

AbstractJob *ClamAvJobConfiguration::CreateJob()
{
    return new ClamAvJob();
}

