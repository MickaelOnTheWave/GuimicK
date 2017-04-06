#ifndef CONFIGURATIONTEST_H
#define CONFIGURATIONTEST_H

#include "qttestsuite.h"

#include "configuration.h"

class ConfigurationTest : public QtTestSuite
{
    Q_OBJECT
public:
    ConfigurationTest(const std::string& dataPrefix);

private Q_SLOTS:
    void init();
    void cleanup();

    void testLoadFromFile_data();
    void testLoadFromFile();

    void testLoadFromFile_Agent_data();
    void testLoadFromFile_Agent();
    void testLoadFromFile_Client_data();
    void testLoadFromFile_Client();
    void testLoadFromFile_Jobs_data();
    void testLoadFromFile_Jobs();

    void testBuildSimpleWorkList();

private:
    void LoadErrorExamples();
    void LoadWarningExamples();
    void LoadValidExamples();

    void CheckSelfIdentity();

    Configuration* configuration;
};

#endif // CONFIGURATIONTEST_H
