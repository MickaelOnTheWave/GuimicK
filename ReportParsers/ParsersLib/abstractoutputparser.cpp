#include "abstractoutputparser.h"

#include <fstream>

using namespace std;

AbstractOutputParser::~AbstractOutputParser()
{
}

bool AbstractOutputParser::ParseFile(const string &inputFile)
{
    string fileContents("");

    if (GetFileContent(inputFile, fileContents) == false)
        return false;

    if (ParseBuffer(fileContents) == false)
        return false;

    return true;
}

void AbstractOutputParser::WriteFullDescriptionToFile(const string &filename)
{
    ofstream outFile(filename.c_str());
    outFile << GetFullDescription();
    outFile.close();
}

bool AbstractOutputParser::GetFileContent(const std::string& fileName, string& fileContents)
{
    ifstream inFile(fileName.c_str());

    if (!inFile.is_open())
        return false;

    fileContents = "";

    string line;
    while (getline(inFile, line))
        fileContents += line + "\n";

    inFile.close();
    return true;
}
