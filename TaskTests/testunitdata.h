#ifndef TESTUNITDATA_H
#define TESTUNITDATA_H

#include <string>
#include <vector>

class TestUnitData
{
public:
    TestUnitData();
    TestUnitData(const std::string& file);   

    void PopulateFromOutputFile(const std::string& file);

    std::string GetName(void) const;
    int GetOkTestCount(void) const;
    int GetFailTestCount(void) const;
    int GetTestCount(void) const;

    bool GetResult(void) const;

    std::vector<std::pair<std::string, bool> >::const_iterator FunctionsBegin() const;
    std::vector<std::pair<std::string, bool> >::const_iterator FunctionsEnd() const;

private:
    void Parse(const std::string& content);

    size_t ParseUnitName(const std::string &content);
    void ParseFunctionText(const std::string& content);
    std::string ParseFunctionName(const std::string& content);
    void ParseIncidentText(const std::string& content, const std::string& functionName);
    bool ParseIncidentResult(const std::string& content);
    std::string ParseIncidentName(const std::string& content);

    void FillElementList(const std::string& elementName, const std::string& content,
                            std::vector<std::string>& outputList);


    size_t GetStartingTagPosition(const std::string& name,
                                  const std::string& content, const size_t position);
    size_t GetEndingTagPosition(const std::string& name,
                                const std::string& content, size_t& position);

    std::string GetCdataValue(const std::string& tagValue);

    std::string GetTagValue(const std::string& content, size_t& position,
                            const std::string& tagStart);
    std::string GetTagValue(const std::string& content, size_t& position,
                            const std::string& tagStart, const std::string& tagEnd);

    std::vector<std::pair<std::string, bool> > functions;
    std::string name = "";
};

#endif // TESTUNITDATA_H
