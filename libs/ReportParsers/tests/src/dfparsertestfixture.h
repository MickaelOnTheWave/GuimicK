#ifndef DFPARSERTESTFIXTURE_H
#define DFPARSERTESTFIXTURE_H

#include "diskparsertestfixture.h"
#include "dfcommandparser.h"

class DfParserTestFixture : public DiskParserTestFixture
{
public:
   DfParserTestFixture() : DiskParserTestFixture()
   {
     parser = new DfCommandParser();
   }
};

#endif // DFPARSERTESTFIXTURE_H
