#include "userconsolejobconfigurationtest.h"

#include <QTest>

#include "testutils.h"
#include "userconsolejob.h"
#include "userconsolejobconfiguration.h"

using namespace std;

void UserConsoleJobConfigurationTest::testConfigure_TitleProperty()
{
    TestTitleProperty("My Testing Title");
}

void UserConsoleJobConfigurationTest::testConfigure_CommandProperty()
{
    TestCommandProperty("command");
}

void UserConsoleJobConfigurationTest::testConfigure_ParamsProperty()
{
    TestParamsProperty("parameters");
}

void UserConsoleJobConfigurationTest::testConfigure_ReturnCodeProperty_data()
{
    QTest::addColumn<QString>("propertyValue");
    QTest::addColumn<int>("expectedValue");

    for (int i=-1; i<3; ++i)
    {
        const QString strCode = QString::number(i);
        QTest::newRow(strCode.toStdString().c_str()) << strCode << i;
    }
}

void UserConsoleJobConfigurationTest::testConfigure_ReturnCodeProperty()
{
    QFETCH(QString, propertyValue);
    QFETCH(int, expectedValue);
    TestReturnCodeProperty(propertyValue.toStdString(), expectedValue);
}

void UserConsoleJobConfigurationTest::testConfigure_ParserUsesBufferProperty_data()
{
    QTest::addColumn<QString>("propertyValue");
    QTest::addColumn<bool>("expectedValue");

    QTest::newRow("true") << "true" << true;
    QTest::newRow("false") << "false" << false;
    QTest::newRow("Default is false") << "" << false;
    QTest::newRow("Unknown is false") << "blabla" << false;
}

void UserConsoleJobConfigurationTest::testConfigure_ParserUsesBufferProperty()
{
    QFETCH(QString, propertyValue);
    QFETCH(bool, expectedValue);
    TestParserUsesBufferProperty(propertyValue.toStdString(), expectedValue);
}

void UserConsoleJobConfigurationTest::testConfigure_UserAttachment_data()
{
   QTest::addColumn<QStringList>("attachments");

   QTest::newRow("No Attachments") << QStringList();
   QTest::newRow("One Attachments") << QStringList{"file01.txt"};
   QTest::newRow("Multiple Attachments") << QStringList{
                                            "att1.txt", "att2.txt",
                                            "haha", "hoho"};
   QTest::newRow("Multiple Attachments with same name")
         << QStringList{ "att1.txt", "att2.txt",
                         "duplicate", "duplicate"};
}

void UserConsoleJobConfigurationTest::testConfigure_UserAttachment()
{
   QFETCH(QStringList, attachments);
   TestUserAttachments(attachments);
}

void UserConsoleJobConfigurationTest::testConfigure_ExpectedOutputProperty()
{
    TestExpectedOutputProperty("expected output");
}

void UserConsoleJobConfigurationTest::testConfigure_OutputFilenameProperty()
{
    TestOutputFilenameProperty("somefile.txt");
}

void UserConsoleJobConfigurationTest::testConfigure_ParserCommandProperty()
{
    TestParserCommandProperty("/bin/parsercommand");
}

AbstractJobConfiguration *UserConsoleJobConfigurationTest::CreateNewConfiguration() const
{
    return new UserConsoleJobConfiguration();
}

void UserConsoleJobConfigurationTest::TestTitleProperty(const string &propertyValue)
{
    ConfigurationObject* confObject = CreateSimpleConfigurationObject(
                UserConsoleJobConfiguration::TitleProperty, propertyValue);

    AbstractJob* job = TestConfigurationWithoutErrors(confObject);

    FinalCheckTitleProperty(job, propertyValue);

    delete job;
    delete confObject;
}

void UserConsoleJobConfigurationTest::TestCommandProperty(const std::string &propertyValue)
{
    ConfigurationObject* confObject = CreateSimpleConfigurationObject(
                UserConsoleJobConfiguration::CommandProperty, propertyValue);

    AbstractJob* job = TestConfigurationWithoutErrors(confObject);
    auto castJob = dynamic_cast<AbstractConsoleJob*>(job);

    QVERIFY(castJob != nullptr);
    QCOMPARE(castJob->GetCommand(), propertyValue);

    delete job;
    delete confObject;
}

void UserConsoleJobConfigurationTest::TestParamsProperty(const string &propertyValue)
{
    ConfigurationObject* confObject = CreateSimpleConfigurationObject(
                UserConsoleJobConfiguration::ParameterProperty, propertyValue);

    AbstractJob* job = TestConfigurationWithoutErrors(confObject);
    auto castJob = dynamic_cast<AbstractConsoleJob*>(job);

    QVERIFY(castJob != nullptr);
    QCOMPARE(castJob->GetCommandParameters(), propertyValue);

    delete job;
    delete confObject;
}

void UserConsoleJobConfigurationTest::TestReturnCodeProperty(const string &propertyValue,
                                                             const int &expectedValue)
{
    ConfigurationObject* confObject = CreateSimpleConfigurationObject(
                UserConsoleJobConfiguration::ReturnCodeProperty, propertyValue);

    AbstractJob* job = TestConfigurationWithoutErrors(confObject);
    auto castJob = dynamic_cast<AbstractConsoleJob*>(job);

    QVERIFY(castJob != nullptr);
    QCOMPARE(castJob->GetExpectedReturnCode(), expectedValue);

    delete job;
    delete confObject;
}

void UserConsoleJobConfigurationTest::TestExpectedOutputProperty(const string &propertyValue)
{
    ConfigurationObject* confObject = CreateSimpleConfigurationObject(
                UserConsoleJobConfiguration::ExpectedOutputProperty, propertyValue);

    AbstractJob* job = TestConfigurationWithoutErrors(confObject);

    FinalCheckExpectedOutputProperty(job, propertyValue);

    delete job;
    delete confObject;
}

void UserConsoleJobConfigurationTest::TestOutputFilenameProperty(const string &propertyValue)
{
    ConfigurationObject* confObject = CreateSimpleConfigurationObject(
                UserConsoleJobConfiguration::OutputFilenameProperty, propertyValue);

    AbstractJob* job = TestConfigurationWithoutErrors(confObject);

    FinalCheckOutputFilenameProperty(job, propertyValue);

    delete job;
    delete confObject;
}

void UserConsoleJobConfigurationTest::TestParserCommandProperty(const string &propertyValue)
{
    ConfigurationObject* confObject = CreateSimpleConfigurationObject(
                UserConsoleJobConfiguration::ParserCommandProperty, propertyValue);

    AbstractJob* job = TestConfigurationWithoutErrors(confObject);

    FinalCheckParserCommandProperty(job, propertyValue);

    delete job;
    delete confObject;
}

void UserConsoleJobConfigurationTest::TestParserUsesBufferProperty(const string &propertyValue,
                                                                   const bool &expectedValue)
{
    ConfigurationObject* confObject = CreateSimpleConfigurationObject(
                UserConsoleJobConfiguration::ParserUsesBufferProperty, propertyValue);

    AbstractJob* job = TestConfigurationWithoutErrors(confObject);

    FinalCheckParserUsesBufferProperty(job, expectedValue);

    delete job;
    delete confObject;
}

void UserConsoleJobConfigurationTest::TestUserAttachments(const QStringList& attachments)
{
   auto confObject = new ConfigurationObject();
   for (auto&& it : attachments)
      AddUserAttachmentObject(confObject, it.toStdString());

   AbstractJob* job = TestConfigurationWithoutErrors(confObject);
   auto castJob = dynamic_cast<UserConsoleJob*>(job);

   QVERIFY(castJob != nullptr);

   vector<string> actualUserAttachments;
   castJob->GetUserAttachments(actualUserAttachments);

   TestUtils::CheckListsAreEqual(attachments, actualUserAttachments);

   delete job;
   delete confObject;
}

void UserConsoleJobConfigurationTest::FinalCheckTitleProperty(AbstractJob *job,
                                                              const string &expectedValue)
{
    auto castJob = dynamic_cast<UserConsoleJob*>(job);
    QVERIFY(castJob != nullptr);
    QCOMPARE(castJob->GetName(), expectedValue);
}

void UserConsoleJobConfigurationTest::FinalCheckExpectedOutputProperty(AbstractJob *job,
                                                                       const string &expectedValue)
{
    auto castJob = dynamic_cast<UserConsoleJob*>(job);
    QVERIFY(castJob != nullptr);
    QCOMPARE(castJob->GetExpectedOutput(), expectedValue);
}

void UserConsoleJobConfigurationTest::FinalCheckOutputFilenameProperty(AbstractJob *job,
                                                                       const string &expectedValue)
{
    auto castJob = dynamic_cast<UserConsoleJob*>(job);
    QVERIFY(castJob != nullptr);
    QCOMPARE(castJob->GetOutputFile(), expectedValue);
}

void UserConsoleJobConfigurationTest::FinalCheckParserCommandProperty(AbstractJob *job,
                                                                      const string &expectedValue)
{
    auto castJob = dynamic_cast<UserConsoleJob*>(job);
    QVERIFY(castJob != nullptr);
    QCOMPARE(castJob->GetMiniDescriptionParserCommand(), expectedValue);
}

void UserConsoleJobConfigurationTest::FinalCheckParserUsesBufferProperty(AbstractJob *job,
                                                                         const bool expectedValue)
{
    auto castJob = dynamic_cast<UserConsoleJob*>(job);
    QVERIFY(castJob != nullptr);
    QCOMPARE(castJob->IsParsingUsingBuffer(), expectedValue);
}

void UserConsoleJobConfigurationTest::AddUserAttachmentObject(ConfigurationObject* confObject,
                                                              const string& name)
{
   auto userAttachmentObject = new ConfigurationObject(UserConsoleJobConfiguration::UserAttachmentObject);
   userAttachmentObject->SetProperty("param0", name);
   confObject->AddObject(userAttachmentObject);
}
