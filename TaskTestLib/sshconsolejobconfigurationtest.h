#ifndef SSHCONSOLEJOBCONFIGURATIONTEST_H
#define SSHCONSOLEJOBCONFIGURATIONTEST_H

#include "userconsolejobconfigurationtest.h"

class SshConsoleJobConfigurationTest : public UserConsoleJobConfigurationTest
{
    Q_OBJECT

public:
    SshConsoleJobConfigurationTest() = default;
    virtual ~SshConsoleJobConfigurationTest() = default;

private Q_SLOTS:

    void testJobIsSsh();

protected:
    virtual AbstractJobConfiguration* CreateNewConfiguration() const override;

private:
    virtual void FinalCheckTitleProperty(AbstractJob* job, const std::string& expectedValue);
    virtual void FinalCheckExpectedOutputProperty(AbstractJob* job, const std::string& expectedValue);
    virtual void FinalCheckOutputFilenameProperty(AbstractJob* job, const std::string& expectedValue);
    virtual void FinalCheckParserCommandProperty(AbstractJob* job, const std::string& expectedValue);
    virtual void FinalCheckParserUsesBufferProperty(AbstractJob* job, const bool expectedValue);
};

#endif // SSHCONSOLEJOBCONFIGURATIONTEST_H
