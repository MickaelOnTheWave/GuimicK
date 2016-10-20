#ifndef ABSTRACTOUTPUTPARSER_H
#define ABSTRACTOUTPUTPARSER_H

#include <string>

class AbstractOutputParser
{
public :
    virtual bool ParseUsingFiles(const std::string& inputFile,
                                 const std::string& outputFile, std::string& description) = 0;
};


#endif // ABSTRACTOUTPUTPARSER_H
