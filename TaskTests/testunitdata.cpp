#include "testunitdata.h"

#include "filetools.h"

using namespace std;

TestUnitData::TestUnitData()
{
}

TestUnitData::TestUnitData(const std::string &file)
{
    PopulateFromOutputFile(file);
}

std::string TestUnitData::GetName() const
{
    return name;
}

int TestUnitData::GetOkTestCount() const
{
    int count = 0;
    for (auto it=functions.begin(); it!=functions.end(); ++it)
        if (it->second == true)
            ++count;
    return count;
}

int TestUnitData::GetFailTestCount() const
{
    int count = 0;
    for (auto it=functions.begin(); it!=functions.end(); ++it)
        if (it->second == false)
            ++count;
    return count;
}

int TestUnitData::GetTestCount() const
{
    return functions.size();
}

bool TestUnitData::GetResult() const
{
    for (auto it=functions.begin(); it!=functions.end(); ++it)
        if (it->second == false)
            return false;
    return true;
}

vector<pair<string, bool> >::const_iterator TestUnitData::FunctionsBegin() const
{
    return functions.begin();
}

vector<pair<string, bool> >::const_iterator TestUnitData::FunctionsEnd() const
{
    return functions.end();
}

void TestUnitData::PopulateFromOutputFile(const std::string &file)
{
    string contents = FileTools::GetTextFileContent(file);
    Parse(contents);
}

void TestUnitData::Parse(const string &content)
{
    size_t position = ParseUnitName(content);

    FindNextFunction(content, position);
    while (position != string::npos)
    {
        ParseFunction(content, position);
        FindNextFunction(content, position);
    }
}

size_t TestUnitData::ParseUnitName(const string &content)
{
    size_t position = 0;
    name = GetTagValue(content, position, "<TestCase name=\"");
    return position;
}

void TestUnitData::FindNextFunction(const string &content, size_t& position)
{
    const string tag = "<TestFunction";
    size_t tagPos = content.find(tag, position);
    position = (tagPos != string::npos) ? tagPos + tag.size() : string::npos;
}

void TestUnitData::ParseFunction(const string &content, size_t &position)
{
    string functionName = GetTagValue(content, position, "name=\"");
    bool functionResult = ParseResult(content, position);
    pair<string, bool> mypair(functionName, functionResult);
    functions.push_back(mypair);
}

bool TestUnitData::ParseResult(const string &content, size_t &position)
{
    string tagContent = GetTagValue(content, position, "<Incident type=\"");
    return (tagContent == "pass");
}

string TestUnitData::GetTagValue(const string &content, size_t &position, const string &tagStart)
{
    return GetTagValue(content, position, tagStart, "\"");
}

string TestUnitData::GetTagValue(const string &content, size_t &position, const string &tagStart, const string &tagEnd)
{
    size_t tagStartPos = content.find(tagStart, position) + tagStart.size();
    size_t tagEndPos = content.find(tagEnd, tagStartPos);
    position = tagEndPos;
    return content.substr(tagStartPos, tagEndPos-tagStartPos);
}

