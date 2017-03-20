#ifndef MOCKPARSER_H
#define MOCKPARSER_H

#include "abstractoutputparser.h"

class MockParser : public AbstractOutputParser
{
public:
    MockParser() = default;
    virtual ~MockParser() = default;

    virtual bool ParseBuffer(const std::string& buffer);

    virtual std::string GetMiniDescription();
    virtual std::string GetFullDescription();

    std::string GetFileContent() const;
    void SetFullDescription(const std::string& description);

private:
    std::string parsedContent = "";
    std::string contentToWrite = "";
};

#endif // MOCKPARSER_H
