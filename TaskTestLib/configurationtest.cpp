#include "configurationtest.h"

#include <QTest>
#include "testutils.h"

using namespace std;

const string suitePrefix = "Configuration/";

ConfigurationTest::ConfigurationTest(const string &dataPrefix) :
    QtTestSuite(dataPrefix + suitePrefix)
{
}

void ConfigurationTest::init()
{
    configuration = new Configuration();
}

void ConfigurationTest::cleanup()
{
    delete configuration;
}

void ConfigurationTest::testLoadFromFile_Errors_data()
{
    QTest::addColumn<QString>("filename");
    QTest::addColumn<bool>("expectedResult");
    QTest::addColumn<QStringList>("expectedErrors");

    LoadRootErrorExamples();
    LoadRootWarningExamples();
    LoadAgentExamples();
    LoadClientExamples();
    LoadJobsExamples();
    LoadRemoteJobListExamples();

    LoadValidExamples();
}

void ConfigurationTest::testLoadFromFile_Errors()
{
    QFETCH(QString, filename);
    QFETCH(bool, expectedResult);
    QFETCH(QStringList, expectedErrors);

    LoadFromFile(filename, expectedResult, expectedErrors);
}

void ConfigurationTest::testLoadFromFile_Agent_Valid()
{
    LoadFromFile("valid.txt", true, QStringList());

    const SelfIdentity* identity = configuration->GetAgent();

    QCOMPARE(identity->name.c_str(), "Testing Agent");
    QCOMPARE(identity->email.c_str(), "invalid.email@provider.com");
    QCOMPARE(identity->emailPassword.c_str(), "itisabadhabittostoreaclearpassword");
    QCOMPARE(identity->emailSmtpServer.c_str(), "smtp.provider.com");
    QCOMPARE(identity->emailSmtpPort, 123);
    QCOMPARE(identity->emailUseSsl, true);
}

void ConfigurationTest::testLoadFromFile_Client_Valid()
{
    LoadFromFile("valid.txt", true, QStringList());

    Client* client = configuration->GetClient();

    QCOMPARE(client->GetName().c_str(), "Machine to manage");
    QCOMPARE(client->GetProperty("mac").c_str(), "00:11:22:33:44:55");
    QCOMPARE(client->GetProperty("ip").c_str(), "10.11.12.13");
    QCOMPARE(client->GetProperty("broadcast").c_str(), "10.11.12.255");
    QCOMPARE(client->GetProperty("sshuser").c_str(), "userusedforremotetasks");
}

void ConfigurationTest::testBuildSimpleWorkList()
{
    LoadFromFile("valid.txt", true, QStringList());

    vector<AbstractJob*> jobList;
    ClientWorkManager* manager = configuration->BuildSimpleWorkList();
    manager->GetJobList(jobList);

    QCOMPARE(jobList.size(), 5ul);
    QCOMPARE(jobList[0]->GetName().c_str(), "Wake");
    QCOMPARE(jobList[1]->GetName().c_str(), "Rsnapshot Backup");
    QCOMPARE(jobList[2]->GetName().c_str(), "ClamAV Scan");
    QCOMPARE(jobList[3]->GetName().c_str(), "Change Screen Saver");
    QCOMPARE(jobList[4]->GetName().c_str(), "Shutdown");
}

void ConfigurationTest::testRemoteJobList()
{
    LoadFromFile("remote - valid.txt", true, QStringList());

    vector<AbstractJob*> jobList;
    ClientWorkManager* manager = configuration->BuildSimpleWorkList();
    manager->GetJobList(jobList);

    QCOMPARE(jobList.size(), 5ul);
    QCOMPARE(jobList[0]->GetName().c_str(), "Wake");
    QCOMPARE(jobList[1]->GetName().c_str(), "Rsnapshot Backup");
    QCOMPARE(jobList[2]->GetName().c_str(), "ClamAV Scan");
    QCOMPARE(jobList[3]->GetName().c_str(), "Change Screen Saver");
    QCOMPARE(jobList[4]->GetName().c_str(), "Shutdown");

}

void ConfigurationTest::LoadRootErrorExamples()
{
    QTest::newRow("Root - Inexistent file")
                            << "inexistent file"
                            << false
                            << QStringList({"File could not be opened"});
    QTest::newRow("Root - Invalid file")
                            << "root - invalid.jpg"
                            << false
                            << QStringList({"Syntax error : expected '{', received '\007'"});
    QTest::newRow("Root - Missing agent")
                            << "root - noagent.txt"
                            << false
                            << QStringList({"Error : missing Agent configuration"});
    QTest::newRow("Root - Missing client")
                            << "root - noclient.txt"
                            << false
                            << QStringList({"Error : missing Client"});
}

void ConfigurationTest::LoadRootWarningExamples()
{
    QTest::newRow("Root - Unsupported master object")
                            << "root - unknown object.txt"
                            << true
                            << QStringList({"Warning : unknown object \"UnknownMasterObject\""});
    QTest::newRow("Root - Invalid boolean value")
                            << "root - invalid boolean value.txt"
                            << true
                            << QStringList({"Warning : maybe is not a valid boolean value. Defaulting to false"});
    QTest::newRow("Root - Unsupported global property")
                            << "root - unknown property.txt"
                            << true
                            << QStringList({"Warning : unknown property \"UnknownGlobalProperty\""});
    QTest::newRow("Root - Unspecified Report")
                            << "root - unspecified report.txt"
                            << true
                            << QStringList({"Warning : missing Report configuration. Defaulting to text"});
    QTest::newRow("Root - Unsupported report type")
                            << "root - unsupported report.txt"
                            << true
                            << QStringList({"Warning : unsupported \"whatever\" report type. Defaulting to text"});
}

void ConfigurationTest::LoadAgentExamples()
{
    QTest::newRow("Agent - missing password")
                            << "agent - nopassword.txt"
                            << true
                            << QStringList({"Error : missing data for email sending. Defaulting to console."});
    QTest::newRow("Agent - unknown property")
                            << "agent - unknown property.txt"
                            << true
                            << QStringList({"Warning : unhandled property unknownProperty for Agent configuration"});

}

void ConfigurationTest::LoadClientExamples()
{
    QTest::newRow("Client - No name")
                            << "client - noname.txt"
                            << false
                            << QStringList({"Error : Client without name"});
    QTest::newRow("Client - No IP")
                            << "client - noip.txt"
                            << false
                            << QStringList({"Error : Client without IP address"});
    QTest::newRow("Client - No Ssh User")
                            << "client - nossh.txt"
                            << false
                            << QStringList({"Error : Client without Ssh user"});
    QTest::newRow("Client - No job list")
                            << "client - no job list.txt"
                            << true
                            << QStringList({"Warning : client without job list",
                                           "Warning : client has an empty job list"});
    QTest::newRow("Client - empty job list")
                            << "client - empty job list.txt"
                            << true
                            << QStringList({"Warning : client has an empty job list"});
    QTest::newRow("Client - Supporting only one client")
                            << "client - oneonly.txt"
                            << true
                            << QStringList({"Warning : only one client is supported for now. "
                                            "Redefining default client"});
}

void ConfigurationTest::LoadJobsExamples()
{
    QTest::newRow("Jobs - unknown job")
                            << "job - unknown.txt"
                            << true
                            << QStringList({"Warning : unknown job \"UnknownJob\". Ignoring..."});

}

void ConfigurationTest::LoadValidExamples()
{
    QTest::newRow("All Valid") << "valid.txt" << true << QStringList();
    QTest::newRow("Production configuration") << "realconf.txt" << true << QStringList();
}

void ConfigurationTest::LoadRemoteJobListExamples()
{
    QTest::newRow("Remote job list - not set up") << "remote - nolist.txt"
                                                  << true
                                                  << QStringList({"Warning : client without job list",
                                                                  "Warning : client has an empty job list"});

    QTest::newRow("Remote job list - missing remote file") << "remote - nofile.txt"
                                                  << false
                                                  << QStringList({"Error : Client configuration file missing"});

    QTest::newRow("Remote job list - invalid host") << "remote - invalidhost.txt"
                                                  << false
                                                  << QStringList({"Error : Client not available"});

    QTest::newRow("Remote job list - user password") << "remote - userprompt.txt"
                                                  << false
                                                  << QStringList({"Error : Client requires password"});

    QTest::newRow("Remote job list - ok") << "remote - valid.txt"
                                          << true
                                          << QStringList();
}

void ConfigurationTest::LoadFromFile(const QString &file, const bool expectedResult,
                                     const QStringList &expectedErrors)
{
    const string fullFilename = GetDataFolder() + file.toStdString();

    vector<string> errors;
    bool result = configuration->LoadFromFile(fullFilename, errors);
    QCOMPARE(result, expectedResult);
    TestUtils::CheckListsAreEqual(expectedErrors, errors);
}
