#include <gtest/gtest.h>

#include "dfparserfixture.h"

using namespace std;

namespace
{
   LogicalDrive BuildDrive(const std::wstring& _name,
                           const std::wstring& _totalSpace,
                           const std::wstring& _usedSpace,
                           const std::wstring& _ratio)
   {
      LogicalDrive drive;
      drive.name = _name;
      drive.totalSpace = _totalSpace;
      drive.usedSpace = _usedSpace;
      drive.ratio = _ratio;
      return drive;
   }
}
/*
void CheckDrive(const Drive& actual, const Drive& expected)
{
   EXPECT_EQ(actual.name, expected.name);
   EXPECT_EQ(actual.totalSpace, expected.totalSpace);
   EXPECT_EQ(actual.usedSpace, expected.usedSpace);
   EXPECT_EQ(actual.ratio, expected.ratio);
}

void CheckDrives(const vector<Drive>& actual, const vector<Drive>& expected)
{
   ASSERT_EQ(actual.size(), expected.size());

   auto itActual = actual.begin();
   auto itExpected = expected.begin();
   for (; itActual != actual.end(); ++itActual, ++itExpected)
      CheckDrive(*itActual, *itExpected);
}

void TestReportParsing(const wstring& input,
                       const vector<Drive>& expectedDrives)
{
   DfCommandParser parser;
   const bool ok = parser.ParseBuffer(input);
   ASSERT_EQ(ok, true);

   vector<Drive> actualDrives;
   parser.GetDrives(actualDrives);

   CheckDrives(actualDrives, expectedDrives);
}*/
/*
TEST(DfCommandParser, doesnt_crash_when_parsing_invalid_buffer)
{
   DfCommandParser parser;

   const wstring buffer = L"Dummy buffer\n"
                          L"With sr@ange symb0lS{}.";

   const bool ok = parser.ParseBuffer(buffer);
   ASSERT_EQ(ok, false);
}

TEST(DfCommandParser, creates_one_drive_report)
{
   const wstring buffer = L"Sist.fichs      Tama  Ocup Livre Uso% Montado em \n"
                          L"/dev/sda2       230G  168G   50G  78% /";

   Drive expectedDrive;
   expectedDrive.name = L"/dev/sda2";
   expectedDrive.totalSpace = L"230 Gb";
   expectedDrive.usedSpace = L"168 Gb";
   expectedDrive.ratio = L"78%";

   const wstring expectedMiniReport = L"";
   const wstring expectedFullReport = L"";

   TestReportParsing(buffer, {expectedDrive}, expectedMiniReport, expectedFullReport);
}

TEST(DfCommandParser, creates_multiple_drive_report)
{
   const wstring buffer = L"Sist.fichs      Tama  Ocup Livre Uso% Montado em\n"
                          L"udev             12G  4.0K   12G   1% /dev\n"
                          L"tmpfs           2.4G  1.6M  2.4G   1% /run\n"
                          L"/dev/sda2       230G  168G   50G  78% /\n"
                          L"none            4.0K     0  4.0K   0% /sys/fs/cgroup\n"
                          L" none            5.0M     0  5.0M   0% /run/lock\n"
                          L"none             12G   76K   12G   1% /run/shm\n"
                          L"none            100M   20K  100M   1% /run/user\n"
                          L"/dev/sda1        37M  6.3M   31M  18% /boot/efi\n"
                          L"/dev/sdb1       334G  268G   67G  81% /mnt/datadrive\n"
                          L"/dev/sdc2       962G   72M  913G   1% /mnt/linuxdata";

   vector<Drive> expectedDrives;
   expectedDrives.push_back(Drive(L"/dev/sda2", L"230 Gb", L"168 Gb", L"78%"));
   expectedDrives.push_back(Drive(L"/dev/sda1", L"37 Mb", L"6.3 Mb", L"18%"));
   expectedDrives.push_back(Drive(L"/dev/sdb1", L"334 Gb", L"268 Gb", L"81%"));
   expectedDrives.push_back(Drive(L"/dev/sdc2", L"962 Gb", L"72 Mb", L"1%"));

   TestReportParsing(buffer, expectedDrives);
}*/

TEST_F(DfParserFixture, doesnt_crash_when_parsing_invalid_buffer)
{
   TestNoCrashOnInvalidBuffer();
}

TEST_F(DfParserFixture, creates_one_drive_report)
{
   const wstring buffer = L"Sist.fichs      Tama  Ocup Livre Uso% Montado em \n"
                          L"/dev/sda2       230G  168G   50G  78% /";

   LogicalDrive expectedDrive = BuildDrive(L"/dev/sda2", L"230 Gb", L"168 Gb", L"78%");

   const wstring expectedMiniReport = L"";
   const wstring expectedFullReport = L"";

   TestParseOk(buffer, {expectedDrive}, expectedMiniReport, expectedFullReport);
}

TEST_F(DfParserFixture, creates_multiple_drive_report)
{
   const wstring buffer = L"Sist.fichs      Tama  Ocup Livre Uso% Montado em\n"
                          L"udev             12G  4.0K   12G   1% /dev\n"
                          L"tmpfs           2.4G  1.6M  2.4G   1% /run\n"
                          L"/dev/sda2       230G  168G   50G  78% /\n"
                          L"none            4.0K     0  4.0K   0% /sys/fs/cgroup\n"
                          L" none            5.0M     0  5.0M   0% /run/lock\n"
                          L"none             12G   76K   12G   1% /run/shm\n"
                          L"none            100M   20K  100M   1% /run/user\n"
                          L"/dev/sda1        37M  6.3M   31M  18% /boot/efi\n"
                          L"/dev/sdb1       334G  268G   67G  81% /mnt/datadrive\n"
                          L"/dev/sdc2       962G   72M  913G   1% /mnt/linuxdata";

   vector<LogicalDrive> expectedDrives;
   expectedDrives.push_back(BuildDrive(L"/dev/sda2", L"230 Gb", L"168 Gb", L"78%"));
   expectedDrives.push_back(BuildDrive(L"/dev/sda1", L"37 Mb", L"6.3 Mb", L"18%"));
   expectedDrives.push_back(BuildDrive(L"/dev/sdb1", L"334 Gb", L"268 Gb", L"81%"));
   expectedDrives.push_back(BuildDrive(L"/dev/sdc2", L"962 Gb", L"72 Mb", L"1%"));

   const wstring expectedMiniReport = L"";
   const wstring expectedFullReport = L"";

   TestParseOk(buffer, expectedDrives, expectedMiniReport, expectedFullReport);
}
