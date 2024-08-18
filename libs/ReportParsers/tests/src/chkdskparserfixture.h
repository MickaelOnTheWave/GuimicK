#ifndef CHKDSKPARSERFIXTURE_H
#define CHKDSKPARSERFIXTURE_H

#include "diskparsertestfixture.h"
#include "chkdskcommandparser.h"

class ChkdskParserFixture : public DiskParserTestFixture
{
public:
   ChkdskParserFixture() : DiskParserTestFixture()
   {
     parser = new ChkdskCommandParser();
   }
};

#endif // CHKDSKPARSERFIXTURE_H
