#ifndef MOCKPARSER_H
#define MOCKPARSER_H

#include "abstractoutputparser.h"

class MockParser : public AbstractOutputParser
{
public:
    MockParser() = default;
    virtual ~MockParser() = default;

    virtual bool ParseBuffer(const std::wstring& buffer);

    virtual std::wstring GetMiniDescription();
    virtual std::wstring GetFullDescription();

    std::wstring GetFileContent() const;
    void SetFullDescription(const std::wstring& description);

private:
    std::wstring parsedContent = L"";
    std::wstring contentToWrite = L"";
};

#endif // MOCKPARSER_H
