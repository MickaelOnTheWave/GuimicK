#include "shutdownjobconfigurationtest.h"

#include "shutdownjobconfiguration.h"

AbstractJobConfiguration *ShutdownJobConfigurationTest::CreateNewConfiguration() const
{
    return new ShutdownJobConfiguration();
}
