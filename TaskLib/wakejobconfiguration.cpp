#include "wakejobconfiguration.h"
#include "wakejob.h"

using namespace std;

WakeJobConfiguration::WakeJobConfiguration() : AbstractJobConfiguration("Wake")
{
}

AbstractJob *WakeJobConfiguration::CreateJob()
{
    return new WakeJob();
}
