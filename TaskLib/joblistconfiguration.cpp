#include "joblistconfiguration.h"

using namespace std;

JobListConfiguration::JobListConfiguration()
{
}

void JobListConfiguration::FillRootObjects(const std::list<ConfigurationObject *> &objectList,
                                           std::vector<string> &errorMessages)
{

}

void JobListConfiguration::FillGlobalProperties(ConfigurationObject *object,
                                                std::vector<string> &errorMessages)
{

}

bool JobListConfiguration::IsConfigurationConsistent(std::vector<string> &errorMessages)
{
    return false;
}
