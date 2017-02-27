#ifndef DFCOMMANDPARSER_H
#define DFCOMMANDPARSER_H

#include "abstractoutputparser.h"

class DfCommandParser : public AbstractOutputParser
{
public:
    DfCommandParser();

    virtual bool ParseBuffer(const std::string& buffer);

    virtual std::string GetMiniDescription();
    virtual std::string GetFullDescription();
};

#endif // DFCOMMANDPARSER_H
