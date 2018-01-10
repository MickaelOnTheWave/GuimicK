#include "taskmanagerconfiguration.h"

#include "configurationparser.h"
#include "profiledjob.h"

using namespace std;

TaskManagerConfiguration::TaskManagerConfiguration()
    : client(NULL), hasFatalError(false)
{
}

TaskManagerConfiguration::~TaskManagerConfiguration()
{
   delete client;
}

bool TaskManagerConfiguration::LoadFromFile(const string &fileName, vector<string> &errorMessages)
{
    errorMessages.clear();
    Reset();

    ConfigurationParser parser;
    bool result = parser.ParseFile(fileName, errorMessages);
    if (!result)
        return false;

    return SetupData(parser, errorMessages);
}

bool TaskManagerConfiguration::LoadFromBuffer(const string &content, vector<string> &errorMessages)
{
    errorMessages.clear();
    Reset();

    ConfigurationParser parser;
    bool result = parser.ParseBuffer(content, errorMessages);
    if (!result)
        return false;

    return SetupData(parser, errorMessages);
}

bool TaskManagerConfiguration::HasClient() const
{
    return (client != NULL);
}

Client* TaskManagerConfiguration::GetClient() const
{
   return client;
}

void TaskManagerConfiguration::SetClient(Client* _client)
{
   client = _client;
}

string TaskManagerConfiguration::CreateWarning(const string &message) const
{
    return CreateMessage("Warning", message);
}

string TaskManagerConfiguration::CreateError(const string &message) const
{
    return CreateMessage("Error", message);
}

string TaskManagerConfiguration::CreateMessage(const string &tag, const string &message) const
{
    if (message != "")
        return tag + " : " + message;
    else
        return string("");
}

void TaskManagerConfiguration::Reset()
{
   jobList.clear();
   hasFatalError = false;
}

bool TaskManagerConfiguration::SetupData(const ConfigurationParser &parser,
                                         std::vector<string> &errorMessages)
{
    FillRootObjects(parser.objectList, errorMessages);
    FillGlobalProperties(parser.anonymousObject, errorMessages);
    return IsConfigurationConsistent(errorMessages);
}
