#ifndef ABSTRACTJOBCONFIGURATIONTEST_H
#define ABSTRACTJOBCONFIGURATIONTEST_H

#include "qttestsuite.h"

#include "abstractjobconfiguration.h"

class AbstractJobConfigurationTest : public QtTestSuite
{
    Q_OBJECT
public:
    AbstractJobConfigurationTest();
    virtual ~AbstractJobConfigurationTest();

private Q_SLOTS:
    void testConfigure_NullConfiguration();
    void testConfigure_UnknownProperty();
    void testConfigure_UnknownSubObject();

protected:
    AbstractJob *TestConfiguration(ConfigurationObject* confObject,
                                   const std::vector<std::string>& expectedErrorMessages);
    AbstractJob *TestConfigurationWithoutErrors(ConfigurationObject* confObject);

    virtual AbstractJobConfiguration* CreateNewConfiguration() const = 0;

    ConfigurationObject* CreateSimpleConfigurationObject(
            const std::string& property, const std::string& value);

private:
    AbstractJob* RunConfiguration(ConfigurationObject* confObject,
                                  std::vector<std::string> &errorMessages);
    void CheckErrorMessages(const std::vector<std::string> &errorMessages,
                            const std::vector<std::string> &expectedErrorMessages);

    std::string BuildUnknownError(const std::string& object,
                                  const std::string& name) const;

};

#endif // ABSTRACTJOBCONFIGURATIONTEST_H
