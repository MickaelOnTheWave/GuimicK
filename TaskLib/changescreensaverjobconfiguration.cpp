#include "changescreensaverjobconfiguration.h"
#include "changescreensaverjob.h"

#include <stdlib.h>

using namespace std;

ChangeScreensaverJobConfiguration::ChangeScreensaverJobConfiguration()
    : AbstractJobConfiguration("ChangeScreenSaver")
{
}

AbstractJob *ChangeScreensaverJobConfiguration::CreateConfiguredJobAfterCheck(
                                                    ConfigurationObject *confObject,
                                                    vector<string> &)
{
    int time = 600;
    string param = confObject->propertyList["param0"];
    if (param != "")
        time = atoi(param.c_str());
    return new ChangeScreensaverJob(time);
}

void ChangeScreensaverJobConfiguration::FillKnownProperties(std::vector<string> &properties)
{
    properties.push_back("timeout");
}
