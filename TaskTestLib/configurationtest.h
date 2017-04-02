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

private:
    void LoadErrorExamples();
    void LoadWarningExamples();
    void LoadValidExamples();

    void TestLoadedData();
    void CheckSelfIdentity();

    Configuration* configuration;
};

#endif // CONFIGURATIONTEST_H
