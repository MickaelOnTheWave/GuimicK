#include <gtest/gtest.h>

#include "dfcommandparser.h"

using namespace std;

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
   DfCommandParser parser;

   const wstring buffer = L"Sist.fichs      Tama  Ocup Livre Uso% Montado em \n"
                          L"/dev/sda2       230G  168G   50G  78% /";

   const bool ok = parser.ParseBuffer(buffer);
   ASSERT_EQ(ok, true);

   Drive expectedDrive;
   expectedDrive.name = L"/dev/sda2";
   expectedDrive.totalSpace = L"230 Gb";
   expectedDrive.usedSpace = L"168 Gb";
   expectedDrive.ratio = L"78%";

   vector<Drive> expectedDrives = {expectedDrive};
   vector<Drive> actualDrives;
   parser.GetDrives(actualDrives);

   CheckDrives(actualDrives, expectedDrives);
}

TEST(DfCommandParser, creates_multiple_drive_report)
{
   FAIL();
}
