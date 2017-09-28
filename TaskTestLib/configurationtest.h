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

    void testLoadFromFile_Errors_data();
    void testLoadFromFile_Errors();

    void testLoadFromFile_Agent_Valid();
    void testLoadFromFile_Client_Valid();

    void testBuildSimpleWorkList();

    //void testRemoteJobList();

private:
    void LoadRootErrorExamples();
    void LoadRootWarningExamples();
    void LoadAgentExamples();
    void LoadClientExamples();
    void LoadJobsExamples();
    void LoadValidExamples();
    void LoadRemoteJobListExamples();

    void LoadFromFile(const QString& file, const bool expectedResult,
                      const QStringList& expectedErrors);

    Configuration* configuration;
};

#endif // CONFIGURATIONTEST_H
