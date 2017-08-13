#include "changescreensaverjobconfiguration.h"
#include "changescreensaverjob.h"

#include <stdlib.h>

using namespace std;

static const string timeoutProperty = "timeout";

ChangeScreensaverJobConfiguration::ChangeScreensaverJobConfiguration()
    : AbstractJobConfiguration("ChangeScreenSaver")
{
}

AbstractJob *ChangeScreensaverJobConfiguration::CreateJob()
{
    return new ChangeScreensaverJob();
}

void ChangeScreensaverJobConfiguration::ConfigureJob(AbstractJob *job,
                                                     ConfigurationObject *confObject,
                                                     vector<string> &errorMessages)
{
    AbstractJobConfiguration::ConfigureJob(job, confObject, errorMessages);

    int time = 600;
    string param = confObject->GetFirstProperty("param0", timeoutProperty);
    if (param != "")
        time = atoi(param.c_str());

    ChangeScreensaverJob* castJob = static_cast<ChangeScreensaverJob*>(job);
    castJob->SetTimeout(time);
}

void ChangeScreensaverJobConfiguration::FillKnownProperties(std::vector<string> &properties)
{
    properties.push_back(timeoutProperty);
}
