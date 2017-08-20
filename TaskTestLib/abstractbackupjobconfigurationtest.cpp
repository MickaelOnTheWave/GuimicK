#include "abstractbackupjobconfigurationtest.h"

#include <QString>
#include <QTest>
#include "abstractbackupjob.h"
#include "abstractbackupjobconfiguration.h"
#include "qttools.h"

using namespace std;

void AbstractBackupJobConfigurationTest::initTestCase()
{
    backupItemName = GetConfigurationBackupItemName();
}

void AbstractBackupJobConfigurationTest::testConfigure_TargetProperty_data()
{
    QTest::addColumn<QString>("propertyValue");
    QTest::addColumn<bool>("expectedValue");

    QTest::newRow("Local") << "local" << true;
    QTest::newRow("Remote") << "remote" << false;
    QTest::newRow("Remote as default") << "" << false;
    QTest::newRow("Remote on unknown") << "blabla" << false;
}

void AbstractBackupJobConfigurationTest::testConfigure_TargetProperty()
{
    QFETCH(QString, propertyValue);
    QFETCH(bool, expectedValue);

    testConfigure_TargetProperty(propertyValue.toStdString(), expectedValue);
}

void AbstractBackupJobConfigurationTest::testConfigure_JoinReports_data()
{
    QTest::addColumn<QString>("propertyValue");
    QTest::addColumn<bool>("expectedValue");

    QTest::newRow("True") << "true" << true;
    QTest::newRow("False") << "false" << false;
    QTest::newRow("True as default") << "" << true;
    QTest::newRow("True on unknown") << "blabla" << true;
}

void AbstractBackupJobConfigurationTest::testConfigure_JoinReports()
{
    QFETCH(QString, propertyValue);
    QFETCH(bool, expectedValue);

    testConfigure_JoinReportsProperty(propertyValue.toStdString(), expectedValue);
}

void AbstractBackupJobConfigurationTest::testConfigure_BackupItemObjects_data()
{
    QTest::addColumn<QStringList>("properties");
    QTest::addColumn<QStringList>("values");
    QTest::addColumn<QString>("expectedSource");
    QTest::addColumn<QString>("expectedDestination");
    QTest::addColumn<QStringList>("errors");

    QTest::newRow("Valid backup") << QStringList{"source", "dest"}
                                  << QStringList{"valueSrc", "valueDst"}
                                  << "valueSrc" << "valueDst" << QStringList();

    QTest::newRow("No source") << QStringList{"source", "dest"}
                                  << QStringList{"", "valueDst"}
                                  << "" << ""
                                  << QStringList{"Error : source is invalid"};

    QTest::newRow("No destination") << QStringList{"source", "dest"}
                                  << QStringList{"valueSrc", ""}
                                  << "" << ""
                                  << QStringList{"Error : destination is invalid"};
    QTest::newRow("Unknown properties ignored") << QStringList{"source", "dest", "other"}
                                                << QStringList{"valueSrc", "valueDst", "otherVal"}
                                                << "valueSrc" << "valueDst" << QStringList();

}

void AbstractBackupJobConfigurationTest::testConfigure_BackupItemObjects()
{
    QFETCH(QStringList, properties);
    QFETCH(QStringList, values);
    QFETCH(QString, expectedSource);
    QFETCH(QString, expectedDestination);
    QFETCH(QStringList, errors);

    BackupValues backupValues = BuildSingleBackupItem(properties, values);
    AbstractBackupJob::BackupCollection expectedItems = BuildOneItemBackupCollection(
                expectedSource, expectedDestination);

    testConfigure_BackupItemObjects(backupValues, expectedItems, QtTools::ToStdStringVector(errors));
}

void AbstractBackupJobConfigurationTest::testConfigure_TargetProperty(const string &propertyValue,
                                                                      const bool expectedValue)
{
    ConfigurationObject* confObject = new ConfigurationObject();
    if (propertyValue != "")
        confObject->SetProperty(AbstractBackupJobConfiguration::TargetProperty, propertyValue);

    vector<string> expectedErrors;
    AbstractJob* job = TestConfiguration(confObject, expectedErrors);
    auto castJob = dynamic_cast<AbstractBackupJob*>(job);

    QVERIFY(castJob != nullptr);
    QVERIFY(castJob->IsTargetLocal() == expectedValue);

    delete job;
    delete confObject;
}

void AbstractBackupJobConfigurationTest::testConfigure_JoinReportsProperty(const string &propertyValue,
                                                                        const bool expectedValue)
{
    ConfigurationObject* confObject = new ConfigurationObject();
    if (propertyValue != "")
        confObject->SetProperty(AbstractBackupJobConfiguration::JoinReportsProperty, propertyValue);

    vector<string> expectedErrors;
    AbstractJob* job = TestConfiguration(confObject, expectedErrors);
    auto castJob = dynamic_cast<AbstractBackupJob*>(job);

    QVERIFY(castJob != nullptr);
    QVERIFY(castJob->IsReportJoint() == expectedValue);

    delete job;
    delete confObject;
}

void AbstractBackupJobConfigurationTest::testConfigure_BackupItemObjects(
        const BackupValues& values, const AbstractBackupJob::BackupCollection& expectedItems,
        const vector<string>& expectedErrors)
{
    auto confObject = CreateBackupConfigurationObject(values);
    AbstractJob* job = TestConfiguration(confObject, expectedErrors);

    auto castJob = dynamic_cast<AbstractBackupJob*>(job);
    QVERIFY(castJob != nullptr);

    AbstractBackupJob::BackupCollection backupItems;
    castJob->GetFolderList(backupItems);

    CheckCollectionsAreEqual(backupItems, expectedItems);

    delete job;
    delete confObject;
}

ConfigurationObject *AbstractBackupJobConfigurationTest::CreateBackupConfigurationObject(
        const BackupValues& values)
{
    auto configuration = new ConfigurationObject();

    for (auto&& it : values)
        configuration->AddObject(CreateBackupItemObject(it.first, it.second));

    return configuration;
}

ConfigurationObject *AbstractBackupJobConfigurationTest::CreateBackupItemObject(
        const std::vector<string> &properties, const std::vector<string> &values)
{
    auto backupObject = new ConfigurationObject(backupItemName);

    auto itValue = values.begin();
    auto itProp = properties.begin();
    for (; itProp != properties.end(); ++itProp, ++itValue)
        backupObject->SetProperty(*itProp, *itValue);

    return backupObject;
}

void AbstractBackupJobConfigurationTest::CheckCollectionsAreEqual(
        const AbstractBackupJob::BackupCollection &items,
        const AbstractBackupJob::BackupCollection &expectedItems)
{
    QCOMPARE(items.size(), expectedItems.size());

    auto itResult = items.begin();
    auto itExpected = expectedItems.begin();
    for (; itResult != items.end(); ++itResult, ++itExpected)
    {
        if (*itResult != *itExpected)
            QFAIL("Backup items are not as expected");
    }
}

AbstractBackupJobConfigurationTest::BackupValues AbstractBackupJobConfigurationTest::BuildSingleBackupItem(
        const QStringList& properties,
        const QStringList& values)
{
    BackupValues backupValues;
    const vector<string> stdProperties = QtTools::ToStdStringVector(properties);
    const vector<string> stdValues = QtTools::ToStdStringVector(values);
    backupValues.push_back(make_pair(stdProperties, stdValues));
    return backupValues;
}

AbstractBackupJob::BackupCollection AbstractBackupJobConfigurationTest::BuildOneItemBackupCollection(
        const QString &source, const QString &destination)
{
    AbstractBackupJob::BackupCollection collection;
    if (source != "" || destination != "")
        collection.push_back(make_pair(source.toStdString(), destination.toStdString()));
    return collection;
}

string AbstractBackupJobConfigurationTest::GetConfigurationBackupItemName() const
{
    auto configuration = CreateNewConfiguration();
    auto castConfiguration = static_cast<AbstractBackupJobConfiguration*>(configuration);
    string backupItemName = castConfiguration->GetBackupItemName();

    delete configuration;
    return backupItemName;
}
