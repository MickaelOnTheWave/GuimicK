#ifndef REMOTEJOBSRUNNERCONFIGURATIONTEST_H
#define REMOTEJOBSRUNNERCONFIGURATIONTEST_H

#include "abstractjobconfigurationtest.h"

class RemoteJobsRunnerConfigurationTest : public AbstractJobConfigurationTest
{
   Q_OBJECT

public:
   RemoteJobsRunnerConfigurationTest() = default;
   //RemoteJobsRunnerConfigurationTest();
   virtual ~RemoteJobsRunnerConfigurationTest() = default;

private Q_SLOTS:
    void testConfigure_ConfFileProperty();

    void testConfigure_TimedRunProperty_data();
    void testConfigure_TimedRunProperty();

protected:
   AbstractJobConfiguration* CreateNewConfiguration() const override;

private:
    void TestConfFileProperty(const std::string& propertyValue);
    void TestTimedRunProperty(const std::string& propertyValue, const bool expectedValue);
};

#endif // REMOTEJOBSRUNNERCONFIGURATIONTEST_H
