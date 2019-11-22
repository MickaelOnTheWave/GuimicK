#ifndef DFPARSERFIXTURE_H
#define DFPARSERFIXTURE_H

#include "diskcommandparsertest.h"
#include "dfcommandparser.h"

class DfParserFixture : public DiskCommandParserTest
{
public:
    void SetUp() override
    {
        parser = new DfCommandParser();
    }
};

#endif // DFPARSERFIXTURE_H
