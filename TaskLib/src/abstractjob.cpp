#include "abstractjob.h"

using namespace std;

AbstractJob::AbstractJob(const string& _title)
    : debugManager(new JobDebugInformationManager(false)),
      isDebugManagerParent(false), title(_title)
{
}

AbstractJob::AbstractJob(const AbstractJob &other)
    : isDebugManagerParent(other.isDebugManagerParent),
      title(other.title)
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

string AbstractJob::GetName() const
{
   return title;
}

void AbstractJob::SetName(const string& value)
{
   title = value;
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

bool AbstractJob::InitializeFromClient(Client *)
{
    debugManager->SetJobName(GetName());
    return true;
}

int AbstractJob::GetOutputDebugInformationValue() const
{
    return debugManager->GetUse();
}
