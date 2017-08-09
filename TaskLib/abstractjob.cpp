#include "abstractjob.h"

AbstractJob::AbstractJob()
{
}

AbstractJob::~AbstractJob()
{
}

void AbstractJob::SetOutputDebugInformation(const int)
{
}

std::string AbstractJob::GetAttachmentName()
{
    return GetName() + ".txt";
}
