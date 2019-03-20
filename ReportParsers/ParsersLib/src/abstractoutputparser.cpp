#include "abstractoutputparser.h"

#include <fstream>

#include "filetools.h"

using namespace std;

AbstractOutputParser::~AbstractOutputParser()
{
}

bool AbstractOutputParser::ParseFile(const wstring &inputFile)
{
   wstring fileContents(L"");

    if (FileTools::GetTextFileContent(inputFile, fileContents) == false)
        return false;

    if (ParseBuffer(fileContents) == false)
        return false;

    return true;
}

void AbstractOutputParser::WriteFullDescriptionToFile(const wstring &filename)
{
    wofstream outFile(filename.c_str());
    outFile << GetFullDescription();
    outFile.close();
}
