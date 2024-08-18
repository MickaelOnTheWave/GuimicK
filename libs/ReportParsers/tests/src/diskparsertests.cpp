#include "diskcommandparsertest.h"

using namespace std;

namespace
{
   void CheckDrive(const LogicalDrive& actual, const LogicalDrive& expected)
   {
      EXPECT_EQ(actual.name, expected.name);
      EXPECT_EQ(actual.totalSpace, expected.totalSpace);
      EXPECT_EQ(actual.usedSpace, expected.usedSpace);
      EXPECT_EQ(actual.ratio, expected.ratio);
      EXPECT_EQ(actual.badSectors, expected.badSectors);
   }
}

void DiskCommandParserTest::TearDown()
{
    delete parser;
}

void DiskCommandParserTest::TestNoCrashOnInvalidBuffer()
{
    const wstring buffer = L"Dummy buffer\n"
                           L"With sr@ange symb0lS{}.";

    const bool ok = parser->ParseBuffer(buffer);
    ASSERT_EQ(ok, false);
}

void DiskCommandParserTest::TestParseOk(const wstring& inputBuffer,
                                        const DiskOutput& expectedOutput)
{
   TestParse(inputBuffer, expectedOutput, true);
}

void DiskCommandParserTest::TestParseError(const wstring& inputBuffer,
                                           const DiskOutput& expectedOutput)
{
   TestParse(inputBuffer, expectedOutput, false);
}

void DiskCommandParserTest::TestParse(const wstring& inputBuffer,
                                      const DiskOutput& expectedOutput,
                                      const bool expectedResult)
{
   const bool ok = parser->ParseBuffer(inputBuffer);
   ASSERT_EQ(ok, expectedResult);

   vector<LogicalDrive> actualDrives;
   parser->GetDrives(actualDrives);

   CheckDrives(actualDrives, expectedOutput.drives);
   CheckReports(expectedOutput.miniReport, expectedOutput.fullReport);
}

void DiskCommandParserTest::CheckDrives(const vector<LogicalDrive>& actual,
                                        const vector<LogicalDrive>& expected)
{
   ASSERT_EQ(actual.size(), expected.size());

   auto itActual = actual.begin();
   auto itExpected = expected.begin();
   for (; itActual != actual.end(); ++itActual, ++itExpected)
      CheckDrive(*itActual, *itExpected);
}

void DiskCommandParserTest::CheckReports(const wstring& miniReport,
                                         const wstring& fullReport)
{
   EXPECT_EQ(parser->GetMiniDescription(), miniReport);
   EXPECT_EQ(parser->GetFullDescription(), fullReport);
}
