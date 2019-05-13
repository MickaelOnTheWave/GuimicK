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

    testConfigure_TargetProperty(propertyValue.toStdWString(), expectedValue);
}

void AbstractBackupJobConfigurationTest::testConfigure_JoinReports_data()
{
    QTest::addColumn<QString>("propertyValue");
    QTest::addColumn<bool>("expectedValue");

    QTest::newRow("True") << "true" << true;
    QTest::newRow("False") << "false" << false;
    QTest::newRow("False as default") << "" << false;
    QTest::newRow("False on unknown") << "blabla" << false;
}

void AbstractBackupJobConfigurationTest::testConfigure_JoinReports()
{
    QFETCH(QString, propertyValue);
    QFETCH(bool, expectedValue);

    testConfigure_JoinReportsProperty(propertyValue.toStdWString(), expectedValue);
}

void AbstractBackupJobConfigurationTest::testConfigure_BackupItemObjects_data()
{
    QTest::addColumn<QStringList>("properties");
    QTest::addColumn<QStringList>("values");
    QTest::addColumn<QString>("expectedSource");
    QTest::addColumn<QString>("expectedDestination");
    QTest::addColumn<QStringList>("errors");

    CreateBackupItemObjectsTestData();
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

    testConfigure_BackupItemObjects(backupValues, expectedItems, QtTools::ToStdWStringVector(errors));
}

void AbstractBackupJobConfigurationTest::CreateBackupItemObjectsTestData()
{
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

void AbstractBackupJobConfigurationTest::testConfigure_TargetProperty(const wstring &propertyValue,
                                                                      const bool expectedValue)
{
    ConfigurationObject* confObject = new ConfigurationObject();
    if (propertyValue != L"")
        confObject->SetProperty(AbstractBackupJobConfiguration::TargetProperty, propertyValue);

    AbstractJob* job = TestConfigurationWithoutErrors(confObject);
    auto castJob = dynamic_cast<AbstractBackupJob*>(job);

    QVERIFY(castJob != nullptr);
    QVERIFY(castJob->IsTargetLocal() == expectedValue);

    delete job;
    delete confObject;
}

void AbstractBackupJobConfigurationTest::testConfigure_JoinReportsProperty(const wstring &propertyValue,
                                                                        const bool expectedValue)
{
    ConfigurationObject* confObject = new ConfigurationObject();
    if (propertyValue != L"")
        confObject->SetProperty(AbstractBackupJobConfiguration::JoinReportsProperty, propertyValue);

    AbstractJob* job = TestConfigurationWithoutErrors(confObject);
    auto castJob = dynamic_cast<AbstractBackupJob*>(job);

    QVERIFY(castJob != nullptr);
    QVERIFY(castJob->GetJoinReports() == expectedValue);

    delete job;
    delete confObject;
}

void AbstractBackupJobConfigurationTest::testConfigure_BackupItemObjects(
        const BackupValues& values, const AbstractBackupJob::BackupCollection& expectedItems,
        const vector<wstring>& expectedErrors)
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
        const vector<wstring> &properties, const vector<wstring> &values)
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
    const vector<wstring> stdProperties = QtTools::ToStdWStringVector(properties);
    const vector<wstring> stdValues = QtTools::ToStdWStringVector(values);
    backupValues.push_back(make_pair(stdProperties, stdValues));
    return backupValues;
}

AbstractBackupJob::BackupCollection AbstractBackupJobConfigurationTest::BuildOneItemBackupCollection(
        const QString &source, const QString &destination)
{
    AbstractBackupJob::BackupCollection collection;
    if (source != "" || destination != "")
        collection.push_back(make_pair(source.toStdWString(), destination.toStdWString()));
    return collection;
}

wstring AbstractBackupJobConfigurationTest::GetConfigurationBackupItemName() const
{
    auto configuration = CreateNewConfiguration();
    auto castConfiguration = static_cast<AbstractBackupJobConfiguration*>(configuration);
    wstring backupItemName = castConfiguration->GetBackupItemName();

    delete configuration;
    return backupItemName;
}
