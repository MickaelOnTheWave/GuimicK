#ifndef GITFSBACKUPJOBCONFIGURATIONTEST_H
#define GITFSBACKUPJOBCONFIGURATIONTEST_H

#include "abstractjobconfigurationtest.h"

class GitFsBackupJobConfigurationTest : public AbstractJobConfigurationTest
{
    Q_OBJECT

private Q_SLOTS:
    void testConfigure_JoinReports_True();
    void testConfigure_JoinReports_False();
    void testConfigure_JoinReports_TrueAsDefault();
    void testConfigure_JoinReports_TrueOnUnknown();

private:
    virtual AbstractJobConfiguration* CreateNewConfiguration() const override;

    void testConfigure_JoinReportsProperty(const std::string& propertyValue,
                                           const bool expectedValue);
};

#endif // GITFSBACKUPJOBCONFIGURATIONTEST_H
