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

void ConfigurationTest::testLoadFromFile_data()
{
    QTest::addColumn<QString>("filename");
    QTest::addColumn<bool>("expectedResult");
    QTest::addColumn<QStringList>("expectedErrors");

    LoadErrorExamples();
    LoadWarningExamples();
    LoadValidExamples();
}

void ConfigurationTest::testLoadFromFile()
{
    QFETCH(QString, filename);
    QFETCH(bool, expectedResult);
    QFETCH(QStringList, expectedErrors);

    vector<string> errors;
    const string fullFilename = GetDataFolder() + filename.toStdString();
    bool result = configuration->LoadFromFile(fullFilename, errors);
    QCOMPARE(expectedResult, result);
    TestUtils::CheckListsAreEqual(expectedErrors, errors);
}

void ConfigurationTest::testLoadFromFile_Agent_data()
{

}

void ConfigurationTest::testLoadFromFile_Agent()
{
    //CheckSelfIdentity();
    QFAIL("TODO : implement");
}

void ConfigurationTest::testLoadFromFile_Client_data()
{

}

void ConfigurationTest::testLoadFromFile_Client()
{
    QFAIL("TODO : implement");
}

void ConfigurationTest::testLoadFromFile_Jobs_data()
{

}

void ConfigurationTest::testLoadFromFile_Jobs()
{
    QFAIL("TODO : implement");
}

void ConfigurationTest::testBuildSimpleWorkList()
{
    QFAIL("TODO : implement");
}

void ConfigurationTest::LoadErrorExamples()
{
    QTest::newRow("Inexistent file")
                            << "inexistent file"
                            << false
                            << QStringList({"File could not be opened"});
    QTest::newRow("Invalid file")
                            << "image.jpg"
                            << false
                            << QStringList({"Syntax error : expected '{', received '\007'"});
    QTest::newRow("Missing self identity")
                            << "self - missing.txt"
                            << false
                            << QStringList({"Error : missing Agent configuration"});
    QTest::newRow("Missing data for email sending")
                            << "missing email password.txt"
                            << false
                            << QStringList({"Error : missing data for email sending"});
    QTest::newRow("Missing client")
                            << "missing client.txt"
                            << false
                            << QStringList({"Error : missing Client"});
}

void ConfigurationTest::LoadWarningExamples()
{
    QTest::newRow("Unknown property in self identity")
                            << "self - unknown property.txt"
                            << true
                            << QStringList({"Warning : unhandled property unknownProperty for Agent configuration"});
    QTest::newRow("Client without job list")
                            << "no job list.txt"
                            << true
                            << QStringList({"Warning : client without job list"});
    QTest::newRow("Client with empty job list")
                            << "empty job list.txt"
                            << true
                            << QStringList({"Warning : client has an empty job list"});
    QTest::newRow("Unsupported job")
                            << "unknown job.txt"
                            << true
                            << QStringList({"Warning : unknown job \"UnknownJob\". Ignoring..."});
    QTest::newRow("Unsupported master object")
                            << "unknown master object.txt"
                            << true
                            << QStringList({"Warning : unknown object \"UnknownMasterObject\""});
    QTest::newRow("Invalid boolean value")
                            << "invalid boolean value.txt"
                            << true
                            << QStringList({"Warning : maybe is not a valid boolean value. Defaulting to false"});
    QTest::newRow("Unsupported global property")
                            << "unknown global property.txt"
                            << true
                            << QStringList({"Warning : unknown property \"UnknownGlobalProperty\""});
    QTest::newRow("Unspecified Report")
                            << "unspecified report.txt"
                            << true
                            << QStringList({"Warning : missing Report configuration. Defaulting to text"});
    QTest::newRow("Unsupported report type")
                            << "unsupported report.txt"
                            << true
                            << QStringList({"Warning : unsupported \"whatever\" report type. Defaulting to text"});
}

void ConfigurationTest::LoadValidExamples()
{
    QTest::newRow("All Valid") << "valid.txt" << true << QStringList();
}

void ConfigurationTest::CheckSelfIdentity()
{
    QFETCH(QString, expectedName);
    QFETCH(QString, expectedEmail);
    QFETCH(QString, expectedPassword);
    QFETCH(QString, expectedSmtpServer);
    QFETCH(int, expectedSmtpPort);
    QFETCH(bool, expectedSslUse);

    SelfIdentity* identity = configuration->GetSelfIdentity();

    QCOMPARE(identity->name, expectedName.toStdString());
    QCOMPARE(identity->email, expectedEmail.toStdString());
    QCOMPARE(identity->emailPassword, expectedPassword.toStdString());
    QCOMPARE(identity->emailSmtpServer, expectedSmtpServer.toStdString());
    QCOMPARE(identity->emailSmtpPort, expectedSmtpPort);
    QCOMPARE(identity->emailUseSsl, expectedSslUse);
}
