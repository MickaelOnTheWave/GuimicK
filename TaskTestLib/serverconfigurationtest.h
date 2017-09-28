#ifndef SERVERCONFIGURATIONTEST_H
#define SERVERCONFIGURATIONTEST_H

#include "qttestsuite.h"

#include "serverconfiguration.h"

class ServerConfigurationTest : public QtTestSuite
{
    Q_OBJECT
public:
    ServerConfigurationTest(const std::string& dataPrefix);

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

    ServerConfiguration* configuration;
};

#endif // SERVERCONFIGURATIONTEST_H
