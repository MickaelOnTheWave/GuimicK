#include "abstractoutputparser.h"

#include <fstream>

#include "filetools.h"
#include "stringtools.h"

using namespace std;

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
#ifdef _MSC_VER
    wofstream outFile(filename.c_str());
#else
   const string utf8Filename = StringTools::UnicodeToUtf8(filename);
   wofstream outFile(utf8Filename.c_str());
#endif
    outFile << GetFullDescription();
    outFile.close();
}
