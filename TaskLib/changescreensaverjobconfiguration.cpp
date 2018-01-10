#include "changescreensaverjobconfiguration.h"
#include "changescreensaverjob.h"

#include <stdlib.h>

using namespace std;

const string ChangeScreensaverJobConfiguration::TimeoutProperty = "timeout";

ChangeScreensaverJobConfiguration::ChangeScreensaverJobConfiguration()
    : AbstractJobDefaultConfiguration("ChangeScreenSaver")
{
}

bool ChangeScreensaverJobConfiguration::IsRightJob(AbstractJob* job)
{
   return (dynamic_cast<ChangeScreensaverJob*>(job) != NULL);
}

ConfigurationObject* ChangeScreensaverJobConfiguration::CreateConfigurationObject(AbstractJob* job)
{
   ConfigurationObject* confObject = AbstractJobDefaultConfiguration::CreateConfigurationObject(job);
   return confObject;
}

AbstractJob *ChangeScreensaverJobConfiguration::CreateJob()
{
    return new ChangeScreensaverJob();
}

void ChangeScreensaverJobConfiguration::ConfigureJob(AbstractJob *job,
                                                     ConfigurationObject *confObject,
                                                     vector<string> &errorMessages)
{
    AbstractJobDefaultConfiguration::ConfigureJob(job, confObject, errorMessages);

    int time = 600;
    string param = confObject->GetFirstProperty("param0", TimeoutProperty);
    if (param != "")
        time = atoi(param.c_str());

    ChangeScreensaverJob* castJob = static_cast<ChangeScreensaverJob*>(job);
    castJob->SetTimeout(time);
}

void ChangeScreensaverJobConfiguration::FillKnownProperties(std::vector<string> &properties)
{
    AbstractJobDefaultConfiguration::FillKnownProperties(properties);
    properties.push_back(TimeoutProperty);
}
