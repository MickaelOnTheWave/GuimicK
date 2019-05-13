#include "serverconfigurationtest.h"

#include <QTest>

#include "localclientconfiguration.h"
#include "testutils.h"

using namespace std;

namespace
{
   const wstring suitePrefix = L"Configuration/";

   void Compare(const wstring& actual, const string& expected)
   {
      const QString qActual = QString::fromStdWString(actual);
      const QString qExpected = QString::fromStdString(expected);
      QCOMPARE(qActual, qExpected);
   }
}

ServerConfigurationTest::ServerConfigurationTest(const wstring &dataPrefix) :
    QtTestSuite(dataPrefix + suitePrefix)
{
}

void ServerConfigurationTest::init()
{
    configuration = new TaskManagerConfiguration();
}

void ServerConfigurationTest::cleanup()
{
    delete configuration;
}

void ServerConfigurationTest::testLoadFromFile_Errors_data()
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

void ServerConfigurationTest::testLoadFromFile_Errors()
{
    QFETCH(QString, filename);
    QFETCH(bool, expectedResult);
    QFETCH(QStringList, expectedErrors);

    LoadFromFile(filename, expectedResult, expectedErrors);
}

void ServerConfigurationTest::testLoadFromFile_Agent_Valid()
{
    LoadFromFile("valid.txt", true, QStringList());

    const Agent* agent = GetTypedConfiguration()->GetAgent();
    Compare(agent->GetName(), "Testing Agent");

    const EmailData emailData = agent->GetEmailData();
    Compare(emailData.GetAddress(), "invalid.email@provider.com");
    Compare(emailData.GetPassword(), "itisabadhabittostoreaclearpassword");
    Compare(emailData.GetSmtpServer(), "smtp.provider.com");
    QCOMPARE(emailData.GetSmtpPort(), 123);
    QCOMPARE(emailData.GetUseSsl(), true);
}

void ServerConfigurationTest::testLoadFromFile_Client_Valid()
{
    LoadFromFile("valid.txt", true, QStringList());

    Client* client = GetTypedConfiguration()->GetClient();

    Compare(client->GetName(), "Machine to manage");
    Compare(client->GetProperty(L"mac"), "00:11:22:33:44:55");
    Compare(client->GetProperty(L"ip"), "10.11.12.13");
    Compare(client->GetProperty(L"broadcast"), "10.11.12.255");
    Compare(client->GetProperty(L"sshuser"), "userusedforremotetasks");
}

void ServerConfigurationTest::testBuildSimpleWorkList()
{
    LoadFromFile("valid.txt", true, QStringList());

    vector<AbstractJob*> jobList;
    ClientWorkManager* manager = GetTypedConfiguration()->BuildWorkList(false);
    manager->GetJobList(jobList);

    QCOMPARE(jobList.size(), 5ul);
    Compare(jobList[0]->GetName(), "Wake");
    Compare(jobList[1]->GetName(), "Rsnapshot Backup");
    Compare(jobList[2]->GetName(), "ClamAV Scan");
    Compare(jobList[3]->GetName(), "Change Screen Saver");
    Compare(jobList[4]->GetName(), "Shutdown");
}
/*
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
}*/

void ServerConfigurationTest::LoadRootErrorExamples()
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

void ServerConfigurationTest::LoadRootWarningExamples()
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

void ServerConfigurationTest::LoadAgentExamples()
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

void ServerConfigurationTest::LoadClientExamples()
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
                            << QStringList({QString::fromStdWString(LocalClientConfiguration::MsgClientWithoutJobs)});
    QTest::newRow("Client - empty job list")
                            << "client - empty job list.txt"
                            << true
                            << QStringList({"Warning : Client has no jobs"});
    QTest::newRow("Client - Supporting only one client")
                            << "client - oneonly.txt"
                            << true
                            << QStringList({"Warning : only one client is supported for now. "
                                            "Redefining default client"});
}

void ServerConfigurationTest::LoadJobsExamples()
{
    QTest::newRow("Jobs - unknown job")
                            << "job - unknown.txt"
                            << true
                            << QStringList({"Warning : unknown job \"UnknownJob\". Ignoring..."});

}

void ServerConfigurationTest::LoadValidExamples()
{
    QTest::newRow("All Valid") << "valid.txt" << true << QStringList();
    QTest::newRow("Production configuration") << "realconf.txt" << true << QStringList();
}

void ServerConfigurationTest::LoadRemoteJobListExamples()
{
/*    QTest::newRow("Remote job list - not set up") << "remote - nolist.txt"
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
                                          << QStringList();*/

    QTest::newRow("Remote job list - deprecated") << "remote - deprecated.txt"
                                                  << true
                                                  << QStringList({
                                                                 "Warning : Remote option deprecated",
                                                                 "Warning : Client has no jobs"
                                                                 });

}

void ServerConfigurationTest::LoadFromFile(const QString &file, const bool expectedResult,
                                     const QStringList &expectedErrors)
{
    const wstring fullFilename = GetDataFolder() + file.toStdWString();

    vector<wstring> errors;
    const bool result = configuration->LoadFromFile(fullFilename, errors);
    QCOMPARE(result, expectedResult);
    TestUtils::CheckListsAreEqual(expectedErrors, errors);
}

ServerConfiguration* ServerConfigurationTest::GetTypedConfiguration()
{
   return dynamic_cast<ServerConfiguration*>(configuration->GetTypeConfiguration());
}
