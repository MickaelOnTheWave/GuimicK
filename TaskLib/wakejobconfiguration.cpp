#include "wakejobconfiguration.h"
#include "wakejob.h"

using namespace std;

WakeJobConfiguration::WakeJobConfiguration() : AbstractJobConfiguration("Wake")
{
}

AbstractJob *WakeJobConfiguration::CreateConfiguredJobAfterCheck(
                                        ConfigurationObject *confObject,
                                        vector<string> &)
{
    WakeJob* job = new WakeJob();
    string param = confObject->propertyList["param0"];
    if (param == "showDebugInformation")
        job->SetOutputDebugInformation(true);
    return job;
}
