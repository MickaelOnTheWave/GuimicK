#include "taskmanagerconfiguration.h"

#include "configurationparser.h"
#include "profiledjob.h"

using namespace std;

TaskManagerConfiguration::TaskManagerConfiguration()
    : client(NULL), hasFatalError(false)
{
}

bool TaskManagerConfiguration::LoadFromFile(const string &fileName, vector<string> &errorMessages)
{
    errorMessages.clear();
    jobList.clear();
    hasFatalError = false;

    ConfigurationParser parser;
    bool result = parser.ParseFile(fileName, errorMessages);
    if (!result)
        return false;

    FillRootObjects(parser.objectList, errorMessages);
    FillGlobalProperties(parser.anonymousObject, errorMessages);

    return IsConfigurationConsistent(errorMessages);
}

ClientWorkManager *TaskManagerConfiguration::BuildTimedWorkList() const
{
    ClientWorkManager* workManager = new ClientWorkManager(client);

    list<AbstractJob*>::const_iterator it = jobList.begin();
    list<AbstractJob*>::const_iterator end = jobList.end();
    for (; it!=end; it++)
        workManager->AddJob(new ProfiledJob((*it)->Clone()));

    return workManager;
}

ClientWorkManager *TaskManagerConfiguration::BuildSimpleWorkList() const
{
    ClientWorkManager* workManager = new ClientWorkManager(client->Clone());

    list<AbstractJob*>::const_iterator it = jobList.begin();
    list<AbstractJob*>::const_iterator end = jobList.end();
    for (; it!=end; it++)
        workManager->AddJob((*it)->Clone());

    return workManager;
}
