#include "mockparser.h"

using namespace std;

bool MockParser::ParseBuffer(const wstring &buffer)
{
    parsedContent = buffer;
    return true;
}

wstring MockParser::GetMiniDescription()
{
    return wstring(L"dummy mini description");
}

wstring MockParser::GetFullDescription()
{
    return contentToWrite;
}

wstring MockParser::GetFileContent() const
{
    return parsedContent;
}

void MockParser::SetFullDescription(const wstring &description)
{
    contentToWrite = description;
}
