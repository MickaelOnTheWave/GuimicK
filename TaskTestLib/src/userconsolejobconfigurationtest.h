#ifndef USERCONSOLEJOBCONFIGURATIONTEST_H
#define USERCONSOLEJOBCONFIGURATIONTEST_H

#include "abstractjobdefaultconfigurationtest.h"

class UserConsoleJobConfigurationTest : public AbstractJobDefaultConfigurationTest
{
    Q_OBJECT

public:
    UserConsoleJobConfigurationTest() = default;
    virtual ~UserConsoleJobConfigurationTest() = default;

private Q_SLOTS:
    void testConfigure_TitleProperty();
    void testConfigure_CommandProperty();
    void testConfigure_ParamsProperty();
    void testConfigure_ExpectedOutputProperty();
    void testConfigure_OutputFilenameProperty();
    void testConfigure_ParserCommandProperty();

    void testConfigure_ReturnCodeProperty_data();
    void testConfigure_ReturnCodeProperty();

    void testConfigure_ParserUsesBufferProperty_data();
    void testConfigure_ParserUsesBufferProperty();

    void testConfigure_UserAttachment_data();
    void testConfigure_UserAttachment();

protected:
    virtual AbstractJobConfiguration* CreateNewConfiguration() const override;

private:
    void TestTitleProperty(const std::string &propertyValue);
    void TestCommandProperty(const std::string &propertyValue);
    void TestParamsProperty(const std::string &propertyValue);
    void TestReturnCodeProperty(const std::string &propertyValue, const int &expectedValue);
    void TestExpectedOutputProperty(const std::string &propertyValue);
    void TestOutputFilenameProperty(const std::string &propertyValue);
    void TestParserCommandProperty(const std::string &propertyValue);
    void TestParserUsesBufferProperty(const std::string &propertyValue, const bool &expectedValue);
    void TestUserAttachments(const QStringList& attachments);

    virtual void FinalCheckTitleProperty(AbstractJob* job, const std::string& expectedValue);
    virtual void FinalCheckExpectedOutputProperty(AbstractJob* job, const std::string& expectedValue);
    virtual void FinalCheckOutputFilenameProperty(AbstractJob* job, const std::string& expectedValue);
    virtual void FinalCheckParserCommandProperty(AbstractJob* job, const std::string& expectedValue);
    virtual void FinalCheckParserUsesBufferProperty(AbstractJob* job, const bool expectedValue);
    virtual void FinalCheckUserAttachments(AbstractJob* job, const QStringList& attachments);

    void AddUserAttachmentObject(ConfigurationObject* confObject, const std::string& name);
};

#endif // USERCONSOLEJOBCONFIGURATIONTEST_H
