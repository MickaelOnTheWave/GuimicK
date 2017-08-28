#include "clamavjobconfigurationtest.h"
#include "clamavjobconfiguration.h"

AbstractJobConfiguration *ClamAvJobConfigurationTest::CreateNewConfiguration() const
{
    return new ClamAvJobConfiguration();
}
