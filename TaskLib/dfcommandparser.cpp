#include "dfcommandparser.h"

DfCommandParser::DfCommandParser()
{
}

bool DfCommandParser::ParseBuffer(const std::string &buffer)
{
    return (buffer != "wrong buffer");
}

std::string DfCommandParser::GetMiniDescription()
{
    return "No desc for now";
}

std::string DfCommandParser::GetFullDescription()
{
    return "No content for now";
}
