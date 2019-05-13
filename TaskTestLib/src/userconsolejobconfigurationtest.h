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
    void TestTitleProperty(const std::wstring& propertyValue);
    void TestCommandProperty(const std::wstring &propertyValue);
    void TestParamsProperty(const std::wstring& propertyValue);
    void TestReturnCodeProperty(const std::wstring &propertyValue, const int &expectedValue);
    void TestExpectedOutputProperty(const std::wstring& propertyValue);
    void TestOutputFilenameProperty(const std::wstring &propertyValue);
    void TestParserCommandProperty(const std::wstring &propertyValue);
    void TestParserUsesBufferProperty(const std::wstring &propertyValue, const bool &expectedValue);
    void TestUserAttachments(const QStringList& attachments);

    virtual void FinalCheckTitleProperty(AbstractJob* job, const std::wstring& expectedValue);
    virtual void FinalCheckExpectedOutputProperty(AbstractJob* job, const std::wstring& expectedValue);
    virtual void FinalCheckOutputFilenameProperty(AbstractJob* job, const std::wstring& expectedValue);
    virtual void FinalCheckParserCommandProperty(AbstractJob* job, const std::wstring& expectedValue);
    virtual void FinalCheckParserUsesBufferProperty(AbstractJob* job, const bool expectedValue);
    virtual void FinalCheckUserAttachments(AbstractJob* job, const QStringList& attachments);

    void AddUserAttachmentObject(ConfigurationObject* confObject, const std::wstring& name);
};

#endif // USERCONSOLEJOBCONFIGURATIONTEST_H
