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
    return 0;
}

int TestUnitData::GetFailTestCount() const
{
    return GetTestCount();
}

int TestUnitData::GetTestCount() const
{
    return functions.size();
}

bool TestUnitData::GetResult() const
{
    return false;
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
    const string tag = "<TestCase name=\"";
    size_t nameStart = content.find(tag) + tag.size();
    size_t nameEnd = content.find("\"", nameStart);
    name = content.substr(nameStart, nameEnd-nameStart);
    return nameEnd;
}

void TestUnitData::FindNextFunction(const string &content, size_t& position)
{
    const string tag = "<TestFunction";
    size_t tagPos = content.find(tag, position);
    position = (tagPos != string::npos) ? tagPos + tag.size() : string::npos;
}

void TestUnitData::ParseFunction(const string &content, size_t &position)
{
    const string tag = "name=\"";
    size_t tagStart = content.find(tag, position) + tag.size();
    size_t tagEnd = content.find("\"", tagStart);
    string functionName = content.substr(tagStart, tagEnd-tagStart);
    pair<string, bool> mypair(functionName, false);
    functions.push_back(mypair);
    position = tagEnd;
}

