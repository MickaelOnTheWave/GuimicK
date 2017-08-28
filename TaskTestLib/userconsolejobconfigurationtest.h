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
    void testConfigure_ExpectedOutputProperty();
    void testConfigure_OutputFilenameProperty();
    void testConfigure_ParserCommandProperty();

    void testConfigure_ReturnCodeProperty_data();
    void testConfigure_ReturnCodeProperty();

    void testConfigure_ParserUsesBufferProperty_data();
    void testConfigure_ParserUsesBufferProperty();

protected:
    virtual AbstractJobConfiguration* CreateNewConfiguration() const override;

private:
    void TestTitleProperty(const std::string &propertyValue);
    void TestCommandProperty(const std::string &propertyValue);
    void TestReturnCodeProperty(const std::string &propertyValue, const int &expectedValue);
    void TestExpectedOutputProperty(const std::string &propertyValue);
    void TestOutputFilenameProperty(const std::string &propertyValue);
    void TestParserCommandProperty(const std::string &propertyValue);
    void TestParserUsesBufferProperty(const std::string &propertyValue, const bool &expectedValue);
};

#endif // USERCONSOLEJOBCONFIGURATIONTEST_H
