#ifndef CHKDSKPARSERFIXTURE_H
#define CHKDSKPARSERFIXTURE_H

#include "diskcommandparsertest.h"
#include "chkdskcommandparser.h"

class ChkdskParserFixture : public DiskCommandParserTest
{
public:
    void SetUp()
    {
        parser = new ChkdskCommandParser();
    }
};

#endif // CHKDSKPARSERFIXTURE_H
