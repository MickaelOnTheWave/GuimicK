#include "wakejobconfigurationtest.h"

#include "wakejobconfiguration.h"

AbstractJobConfiguration *WakeJobConfigurationTest::CreateNewConfiguration() const
{
    return new WakeJobConfiguration();
}
