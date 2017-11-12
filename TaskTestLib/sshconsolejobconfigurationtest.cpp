#include "sshconsolejobconfigurationtest.h"

#include <QTest>

#include "sshconsolejob.h"
#include "sshconsolejobconfiguration.h"
#include "testutils.h"

using namespace std;

void SshConsoleJobConfigurationTest::testJobIsSsh()
{
    ConfigurationObject* confObject = CreateSimpleConfigurationObject("", "");

    AbstractJob* job = TestConfigurationWithoutErrors(confObject);
    auto castJob = dynamic_cast<SshConsoleJob*>(job);
    QVERIFY(castJob != nullptr);
}

AbstractJobConfiguration *SshConsoleJobConfigurationTest::CreateNewConfiguration() const
{
    return new SshConsoleJobConfiguration();
}

void SshConsoleJobConfigurationTest::FinalCheckTitleProperty(AbstractJob *job,
                                                             const std::string &expectedValue)
{
    auto castJob = dynamic_cast<SshConsoleJob*>(job);
    QVERIFY(castJob != nullptr);
    QCOMPARE(castJob->GetName(), expectedValue);
}

void SshConsoleJobConfigurationTest::FinalCheckExpectedOutputProperty(AbstractJob *job,
                                                                       const string &expectedValue)
{
    auto castJob = dynamic_cast<SshConsoleJob*>(job);
    QVERIFY(castJob != nullptr);
    QCOMPARE(castJob->GetExpectedOutput(), expectedValue);
}

void SshConsoleJobConfigurationTest::FinalCheckOutputFilenameProperty(AbstractJob *job,
                                                                       const string &expectedValue)
{
    auto castJob = dynamic_cast<SshConsoleJob*>(job);
    QVERIFY(castJob != nullptr);
    QCOMPARE(castJob->GetOutputFile(), expectedValue);
}

void SshConsoleJobConfigurationTest::FinalCheckParserCommandProperty(AbstractJob *job,
                                                                      const string &expectedValue)
{
    auto castJob = dynamic_cast<SshConsoleJob*>(job);
    QVERIFY(castJob != nullptr);
    QCOMPARE(castJob->GetMiniDescriptionParserCommand(), expectedValue);
}

void SshConsoleJobConfigurationTest::FinalCheckParserUsesBufferProperty(AbstractJob *job,
                                                                         const bool expectedValue)
{
    auto castJob = dynamic_cast<SshConsoleJob*>(job);
    QVERIFY(castJob != nullptr);
    QCOMPARE(castJob->IsParsingUsingBuffer(), expectedValue);
}

void SshConsoleJobConfigurationTest::FinalCheckUserAttachments(AbstractJob* job,
                                                               const QStringList& attachments)
{
   auto castJob = dynamic_cast<SshConsoleJob*>(job);
   QVERIFY(castJob != nullptr);

   vector<string> actualUserAttachments;
   castJob->GetUserAttachments(actualUserAttachments);

   TestUtils::CheckListsAreEqual(attachments, actualUserAttachments);
}
