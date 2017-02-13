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

    size_t ParseUnitName(const std::string& content);
    void FindNextFunction(const std::string& content, size_t &position);
    void ParseFunction(const std::string& content, size_t& position);
    bool ParseResult(const std::string& content, size_t& position);
    std::string GetTagValue(const std::string& content, size_t& position,
                            const std::string& tagStart);
    std::string GetTagValue(const std::string& content, size_t& position,
                            const std::string& tagStart, const std::string& tagEnd);

    std::vector<std::pair<std::string, bool> > functions;
    std::string name = "";
};

#endif // TESTUNITDATA_H
