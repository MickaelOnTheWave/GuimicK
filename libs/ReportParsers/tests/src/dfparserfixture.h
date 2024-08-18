#ifndef DFPARSERFIXTURE_H
#define DFPARSERFIXTURE_H

#include "diskparsertestfixture.h"
#include "dfcommandparser.h"

class DfParserFixture : public DiskParserTestFixture
{
public:
   DfParserFixture() : DiskParserTestFixture()
   {
     parser = new DfCommandParser();
   }
};

#endif // DFPARSERFIXTURE_H
