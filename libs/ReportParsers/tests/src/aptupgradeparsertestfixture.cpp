#include "aptupgradeparsertestfixture.h"

#include <catch2/catch.hpp>
#include "aptgetupgradeparser.h"

using namespace std;

AptUpgradeParserTestFixture::AptUpgradeParserTestFixture()
  : AbstractOutputParserTestFixture()
{
   parserToTest = new AptGetUpgradeParser();
}

void AptUpgradeParserTestFixture::TestParseOk(const wstring& fileToParse,
                                              const AptGetUpgradeReport& expectedData)
{
   const bool returnValue = parserToTest->ParseFile(dataFolder + fileToParse);
   REQUIRE(returnValue == true);

   auto typedParser = dynamic_cast<AptGetUpgradeParser*>(parserToTest);
   REQUIRE(typedParser != nullptr);

   AptGetUpgradeReport returnedData;
   typedParser->GetReport(returnedData);
   REQUIRE(returnedData == expectedData);
}
