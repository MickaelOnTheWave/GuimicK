#ifndef ABSTRACTJOBCONFIGURATIONTEST_H
#define ABSTRACTJOBCONFIGURATIONTEST_H

#include "abstractjobconfiguration.h"

class AbstractJobConfigurationTest
{
public:
    AbstractJobConfigurationTest();
    virtual ~AbstractJobConfigurationTest() = default;

    void testConfigure_NullConfiguration();
    void testConfigure_UnknownProperty();
    void testConfigure_UnknownSubObject();

protected:
    AbstractJob *TestConfiguration(ConfigurationObject* confObject,
                                   const std::vector<std::wstring>& expectedErrorMessages);
    AbstractJob *TestConfigurationWithoutErrors(ConfigurationObject* confObject);

    virtual AbstractJobConfiguration* CreateNewConfiguration() const = 0;

    ConfigurationObject* CreateSimpleConfigurationObject(
            const std::wstring& property, const std::wstring& value);

private:
    AbstractJob* RunConfiguration(ConfigurationObject* confObject,
                                  std::vector<std::wstring> &errorMessages);
    void CheckErrorMessages(const std::vector<std::wstring> &errorMessages,
                            const std::vector<std::wstring> &expectedErrorMessages);

    std::wstring BuildUnknownError(const std::wstring& object,
                                   const std::wstring& name) const;

};

#endif // ABSTRACTJOBCONFIGURATIONTEST_H
