#include "mockparser.h"

using namespace std;

bool MockParser::ParseBuffer(const string &buffer)
{
    parsedContent = buffer;
    return true;
}

string MockParser::GetMiniDescription()
{
    return string("dummy mini description");
}

string MockParser::GetFullDescription()
{
    return contentToWrite;
}

string MockParser::GetFileContent() const
{
    return parsedContent;
}

void MockParser::SetFullDescription(const string &description)
{
    contentToWrite = description;
}
