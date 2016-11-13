#ifndef ABSTRACTOUTPUTPARSER_H
#define ABSTRACTOUTPUTPARSER_H

#include <string>

class AbstractOutputParser
{
public :
    virtual ~AbstractOutputParser();
    virtual bool ParseBuffer(const std::string& buffer) = 0;
    bool ParseFile(const std::string& inputFile);

    virtual std::string GetMiniDescription() = 0;
    virtual std::string GetFullDescription() = 0;
    void WriteFullDescriptionToFile(const std::string& filename);

protected :
    bool GetFileContent(const std::string& fileName, std::string& fileContents);
};


#endif // ABSTRACTOUTPUTPARSER_H
