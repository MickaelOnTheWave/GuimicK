#include "abstractjob.h"

AbstractJob::AbstractJob()
    : outputDebugInformation(0)
{
}

AbstractJob::~AbstractJob()
{
}

void AbstractJob::SetOutputDebugInformation(const int value)
{
    outputDebugInformation = value;
}
