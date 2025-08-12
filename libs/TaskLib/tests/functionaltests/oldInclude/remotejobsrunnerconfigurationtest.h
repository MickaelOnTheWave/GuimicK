#ifndef REMOTEJOBSRUNNERCONFIGURATIONTEST_H
#define REMOTEJOBSRUNNERCONFIGURATIONTEST_H

#include "abstractjobconfigurationtest.h"

class RemoteJobsRunnerConfigurationTest : public AbstractJobConfigurationTest
{
   Q_OBJECT

public:
   RemoteJobsRunnerConfigurationTest() = default;
   virtual ~RemoteJobsRunnerConfigurationTest() = default;

private Q_SLOTS:
    void testConfigure_ConfFileProperty();

    void testConfigure_TimedRunProperty_data();
    void testConfigure_TimedRunProperty();

protected:
   AbstractJobConfiguration* CreateNewConfiguration() const override;

private:
    void TestConfFileProperty(const std::wstring& propertyValue);
    void TestTimedRunProperty(const std::wstring& propertyValue, const bool expectedValue);
};

#endif // REMOTEJOBSRUNNERCONFIGURATIONTEST_H
