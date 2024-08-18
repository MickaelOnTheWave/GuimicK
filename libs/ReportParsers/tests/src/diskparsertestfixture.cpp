#include "diskparsertestfixture.h"

#include <catch2/catch.hpp>

using namespace std;

namespace
{
void CheckDrive(const LogicalDrive& actual, const LogicalDrive& expected)
{
   REQUIRE( actual.name == expected.name);
   REQUIRE(actual.totalSpace == expected.totalSpace);
   REQUIRE(actual.usedSpace == expected.usedSpace);
   REQUIRE(actual.ratio == expected.ratio);
   REQUIRE(actual.badSectors == expected.badSectors);
}
}

DiskParserTestFixture::~DiskParserTestFixture()
{
   delete parser;
}

void DiskParserTestFixture::TestNoCrashOnInvalidBuffer()
{
   const wstring buffer = L"Dummy buffer\n"
                          L"With sr@ange symb0lS{}.";

   const bool ok = parser->ParseBuffer(buffer);
   REQUIRE(ok == false);
}

void DiskParserTestFixture::TestParseOk(const wstring& inputBuffer,
                                        const DiskOutput& expectedOutput)
{
   TestParse(inputBuffer, expectedOutput, true);
}

void DiskParserTestFixture::TestParseError(const wstring& inputBuffer,
                                           const DiskOutput& expectedOutput)
{
   TestParse(inputBuffer, expectedOutput, false);
}

void DiskParserTestFixture::TestParse(const wstring& inputBuffer,
                                      const DiskOutput& expectedOutput,
                                      const bool expectedResult)
{
   const bool ok = parser->ParseBuffer(inputBuffer);
   REQUIRE(ok == expectedResult);

   vector<LogicalDrive> actualDrives;
   parser->GetDrives(actualDrives);

   CheckDrives(actualDrives, expectedOutput.drives);
   CheckReports(expectedOutput.miniReport, expectedOutput.fullReport);
}

void DiskParserTestFixture::CheckDrives(const vector<LogicalDrive>& actual,
                                        const vector<LogicalDrive>& expected)
{
   REQUIRE(actual.size() == expected.size());

   auto itActual = actual.begin();
   auto itExpected = expected.begin();
   for (; itActual != actual.end(); ++itActual, ++itExpected)
      CheckDrive(*itActual, *itExpected);
}

void DiskParserTestFixture::CheckReports(const wstring& miniReport,
                                         const wstring& fullReport)
{
   REQUIRE(parser->GetMiniDescription() == miniReport);
   REQUIRE(parser->GetFullDescription() == fullReport);
}
