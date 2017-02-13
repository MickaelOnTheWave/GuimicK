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
    ParseUnitName(content);

    vector<string> functionsContent;
    FillElementList("TestFunction", content, functionsContent);

    for (auto it=functionsContent.begin(); it!=functionsContent.end(); ++it)
        ParseFunctionText(*it);
}

void TestUnitData::ParseFunctionText(const string& content)
{
    string functionName = ParseFunctionName(content);
    vector<string> incidentsContent;
    FillElementList("Incident", content, incidentsContent);
    for (auto it=incidentsContent.begin(); it!=incidentsContent.end(); ++it)
        ParseIncidentText(*it, functionName);
}

string TestUnitData::ParseFunctionName(const string &content)
{
    size_t position = 0;
    return GetTagValue(content, position, "name=\"");
}

void TestUnitData::ParseIncidentText(const string &content, const string& functionName)
{
    bool incidentResult = ParseIncidentResult(content);
    string incidentName = ParseIncidentName(content);
    string name = functionName;
    if (incidentName != "")
        name += string(" - ") + incidentName;
    functions.push_back(make_pair(name, incidentResult));
}

bool TestUnitData::ParseIncidentResult(const string &content)
{
    size_t position = 0;
    string result = GetTagValue(content, position, "type=\"");
    return (result == "pass");
}

string TestUnitData::ParseIncidentName(const string &content)
{
    size_t position = 0;
    size_t startTag = GetStartingTagPosition("DataTag", content, position);
    if (startTag == string::npos)
        return string("");

    string tagValue = GetTagValue(content, position, "<DataTag>", "</DataTag>");
    return GetCdataValue(tagValue);
}

void TestUnitData::FillElementList(const string &elementName, const string &content, std::vector<string> &outputList)
{
    size_t currentPosition = 0;
    while (true)
    {
        size_t startTag = GetStartingTagPosition(elementName, content, currentPosition);
        if (startTag == string::npos)
            break;

        size_t endTag = GetEndingTagPosition(elementName, content, startTag);
        outputList.push_back(content.substr(startTag, endTag-startTag));
        currentPosition = endTag;
    }
}

size_t TestUnitData::GetStartingTagPosition(const string &name,
                                            const string &content, const size_t position)
{
    const string startTag = string("<") + name;
    return content.find(startTag, position);
}

size_t TestUnitData::GetEndingTagPosition(const string& name,
                                          const string &content, size_t &position)
{
    size_t oneLinerTagEndPos = content.find('>', position);
    if (oneLinerTagEndPos == string::npos)
        return string::npos;
    else if (content[oneLinerTagEndPos-1] == '/')
        return oneLinerTagEndPos-1;
    else
    {
        const string fullTagEnd = string("</") + name + ">";
        return content.find(fullTagEnd, position);
    }
}

string TestUnitData::GetCdataValue(const string &tagValue)
{
    size_t position = 0;
    return GetTagValue(tagValue, position, "CDATA[", "]");
}

size_t TestUnitData::ParseUnitName(const string& content)
{
    size_t position = 0;
    name = GetTagValue(content, position, "<TestCase name=\"");
    return position;
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

