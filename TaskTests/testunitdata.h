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

private:
    void Parse(const std::string& content);

    size_t ParseUnitName(const std::string& content);
    void FindNextFunction(const std::string& content, size_t &position);
    void ParseFunction(const std::string& content, size_t& position);

    std::vector<std::pair<std::string, bool> > functions;
    std::string name = "";
};

#endif // TESTUNITDATA_H
