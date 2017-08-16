#ifndef ABSTRACTJOBCONFIGURATIONTEST_H
#define ABSTRACTJOBCONFIGURATIONTEST_H

#include "qttestsuite.h"

#include "abstractjobconfiguration.h"

class AbstractJobConfigurationTest : public QtTestSuite
{
    Q_OBJECT
public:
    AbstractJobConfigurationTest(const std::string& dataPrefix,
                                 const std::string& errorPrefix);
    virtual ~AbstractJobConfigurationTest();

private Q_SLOTS:
    void init();
    void cleanup();

    void testConfigure_NullConfiguration();

protected:
    AbstractJob *TestConfiguration(ConfigurationObject* confObject,
                                   std::vector<std::string> &expectedErrorMessages);

private:
    AbstractJob* RunConfiguration(ConfigurationObject* confObject,
                                  std::vector<std::string> &errorMessages);
    void CheckErrorMessages(const std::vector<std::string> &errorMessages,
                            const std::vector<std::string> &expectedErrorMessages);

    virtual AbstractJobConfiguration* CreateNewConfiguration() = 0;
};

#endif // ABSTRACTJOBCONFIGURATIONTEST_H
