#ifndef APTUPGRADEPARSERTESTFIXTURE_H
#define APTUPGRADEPARSERTESTFIXTURE_H

#include "abstractoutputparsertestfixture.h"
#include "aptgetupgradeparser.h"

class AptUpgradeParserTestFixture : public AbstractOutputParserTestFixture
{
public:
   AptUpgradeParserTestFixture();
   virtual ~AptUpgradeParserTestFixture() = default;
   void TestParseOk(const std::wstring& fileToParse, const AptGetUpgradeReport& expectedData);
};

#endif // APTUPGRADEPARSERTESTFIXTURE_H
