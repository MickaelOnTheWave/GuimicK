#include "abstractbackupjobconfigurationtest.h"

#include <catch2/catch.hpp>

#include "abstractbackupjob.h"
#include "abstractbackupjobconfiguration.h"
#include "qttools.h"

using namespace std;

void AbstractBackupJobConfigurationTest::initTestCase()
{
   backupItemName = GetConfigurationBackupItemName();
}

void AbstractBackupJobConfigurationTest::testConfigure_TargetProperty()
{
   testConfigure_TargetProperty(L"local", true);
   testConfigure_TargetProperty(L"remote", false);
   testConfigure_TargetProperty(L"", false);
   testConfigure_TargetProperty(L"blabla", false);
}

void AbstractBackupJobConfigurationTest::testConfigure_JoinReports()
{
   testConfigure_JoinReportsProperty(L"true", true);
   testConfigure_JoinReportsProperty(L"false", false);
   testConfigure_JoinReportsProperty(L"", false);
   testConfigure_JoinReportsProperty(L"blabla", false);
}

void AbstractBackupJobConfigurationTest::testConfigure_BackupItemObjects()
{
   {
      // Valid backup
      auto errors = std::vector<std::wstring>();
      const BackupValues backupValues = BuildSingleBackupItem({L"source", L"dest"}, {L"valueSrc", L"valueDst"});
      AbstractBackupJob::BackupCollection expectedItems = BuildOneItemBackupCollection(L"valueSrc", L"valueDst");
      testConfigure_BackupItemObjects(backupValues, expectedItems, errors);
   }

   {
      // No source
      auto errors = std::vector<std::wstring>{L"Error : source is invalid"};
      const BackupValues backupValues = BuildSingleBackupItem({L"source", L"dest"}, {L"", L"valueDst"});
      AbstractBackupJob::BackupCollection expectedItems = BuildOneItemBackupCollection(L"", L"");
      testConfigure_BackupItemObjects(backupValues, expectedItems, errors);
   }

   {
      // No destination
      auto errors = std::vector<std::wstring>{L"Error : destination is invalid"};
      const BackupValues backupValues = BuildSingleBackupItem({L"source", L"dest"}, {L"valueSrc", L"valueDst"});
      AbstractBackupJob::BackupCollection expectedItems = BuildOneItemBackupCollection(L"", L"");
      testConfigure_BackupItemObjects(backupValues, expectedItems, errors);
   }

   {
      // Unknown properties ignored
      auto errors = std::vector<std::wstring>{L"Error : destination is invalid"};
      const BackupValues backupValues = BuildSingleBackupItem({L"source", L"dest", L"other"},
                                                              {L"valueSrc", L"valueDst", L"otherVal"});
      AbstractBackupJob::BackupCollection expectedItems = BuildOneItemBackupCollection(L"valueSrc", L"valueDst");
      testConfigure_BackupItemObjects(backupValues, expectedItems, errors);
   }
}

void AbstractBackupJobConfigurationTest::testConfigure_TargetProperty(const wstring &propertyValue,
                                                                      const bool expectedValue)
{
    ConfigurationObject* confObject = new ConfigurationObject();
    if (propertyValue != L"")
        confObject->SetProperty(AbstractBackupJobConfiguration::TargetProperty, propertyValue);

    AbstractJob* job = TestConfigurationWithoutErrors(confObject);
    auto castJob = dynamic_cast<AbstractBackupJob*>(job);

    REQUIRE(castJob != nullptr);
    REQUIRE(castJob->IsTargetLocal() == expectedValue);

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

    REQUIRE(castJob != nullptr);
    REQUIRE(castJob->GetJoinReports() == expectedValue);

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
    REQUIRE(castJob != nullptr);

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
    REQUIRE(items.size() == expectedItems.size());

    auto itResult = items.begin();
    auto itExpected = expectedItems.begin();
    for (; itResult != items.end(); ++itResult, ++itExpected)
    {
        if (*itResult != *itExpected)
            FAIL("Backup items are not as expected");
    }
}

AbstractBackupJobConfigurationTest::BackupValues AbstractBackupJobConfigurationTest::BuildSingleBackupItem(
   const std::vector<std::wstring>& properties,
   const std::vector<std::wstring>& values)
{
    BackupValues backupValues;
    backupValues.push_back(make_pair(properties, values));
    return backupValues;
}

AbstractBackupJob::BackupCollection AbstractBackupJobConfigurationTest::BuildOneItemBackupCollection(
    const std::wstring& source, const std::wstring& destination)
{
    AbstractBackupJob::BackupCollection collection;
    if (source != L"" || destination != L"")
        collection.push_back(make_pair(source, destination));
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
