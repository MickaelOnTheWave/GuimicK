#include "abstractjob.h"

using namespace std;

AbstractJob::AbstractJob()
    : debugManager(new JobDebugInformationManager(false)),
      isDebugManagerParent(false)
{
}

AbstractJob::AbstractJob(const AbstractJob &other)
    : isDebugManagerParent(other.isDebugManagerParent)
{
    if (isDebugManagerParent)
        debugManager = other.debugManager;
    else
        debugManager = new JobDebugInformationManager(*other.debugManager);
}

AbstractJob::~AbstractJob()
{
    if (!isDebugManagerParent)
        delete debugManager;
}

void AbstractJob::SetOutputDebugInformation(const int value)
{
    debugManager->SetUse(value);
}

void AbstractJob::SetParentDebugManager(JobDebugInformationManager* manager)
{
    if (manager != NULL)
    {
        if (!isDebugManagerParent)
        {
            isDebugManagerParent = true;
            delete debugManager;
        }
        debugManager = manager;
    }
    else if (isDebugManagerParent)
    {
        isDebugManagerParent = false;
        debugManager = new JobDebugInformationManager(false);
    }
}

std::string AbstractJob::GetAttachmentName()
{
    return GetName() + ".txt";
}

bool AbstractJob::InitializeFromClient(Client *client)
{
    if (client)
    {
        string debugProperty = client->GetProperty("showDebugInformation");
        debugManager->SetUse(DebugOutput::GetValue(debugProperty));
    }

    return true;
}

int AbstractJob::GetOutputDebugInformationValue() const
{
    return debugManager->GetUse();
}
