#include "libwakejob.h"

#include "networkwaker.h"

LibWakeJob::LibWakeJob() : AbstractWakeJob()
{
}

LibWakeJob::LibWakeJob(const LibWakeJob &other)
    : AbstractWakeJob(other)
{
}

LibWakeJob::~LibWakeJob()
{
}

AbstractJob* LibWakeJob::Clone()
{
   return new LibWakeJob(*this);
}

JobStatus* LibWakeJob::SetupWaker()
{
   return new JobStatus(JobStatus::Ok);
}

JobStatus* LibWakeJob::RunWaker()
{
   const bool ok = NetworkWaker::Wake(macAddress, broadcastIp);
   if (ok)
      return new JobStatus(JobStatus::Ok);
   else
      return new JobStatus(JobStatus::Error, "Wake failed");
}

