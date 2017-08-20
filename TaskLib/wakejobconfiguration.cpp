#include "wakejobconfiguration.h"
#include "wakejob.h"

using namespace std;

WakeJobConfiguration::WakeJobConfiguration()
    : AbstractJobDefaultConfiguration("Wake")
{
}

AbstractJob *WakeJobConfiguration::CreateJob()
{
    return new WakeJob();
}
