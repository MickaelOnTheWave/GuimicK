#ifndef ABSTRACTOUTPUTPARSER_H
#define ABSTRACTOUTPUTPARSER_H

#include <string>

class AbstractOutputParser
{
public :
    virtual ~AbstractOutputParser() = default;
    virtual bool ParseBuffer(const std::wstring& buffer) = 0;
    bool ParseFile(const std::wstring& inputFile);

    virtual std::wstring GetMiniDescription() = 0;
    virtual std::wstring GetFullDescription() = 0;
    void WriteFullDescriptionToFile(const std::wstring& filename);
};


#endif // ABSTRACTOUTPUTPARSER_H
