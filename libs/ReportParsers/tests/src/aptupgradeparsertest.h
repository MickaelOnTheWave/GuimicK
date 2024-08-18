#ifndef APTUPGRADEPARSERTEST_H
#define APTUPGRADEPARSERTEST_H

#include "abstractoutputparsertest.h"
#include "aptgetupgradeparser.h"

class AptUpgradeParserTest : public AbstractOutputParserTest
{
public:
   AptUpgradeParserTest(const std::wstring& dataPrefix);
   virtual ~AptUpgradeParserTest() = default;
   void testParse();

private:
    AptGetUpgradeReport CreateExpectedData() const;
    AptGetUpgradeReport report;
};

#endif // APTUPGRADEPARSERTEST_H
