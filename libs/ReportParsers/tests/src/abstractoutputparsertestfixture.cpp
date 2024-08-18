#include "abstractoutputparsertestfixture.h"

#include <catch2/catch.hpp>
#include <filetools.h>

AbstractOutputParserTestFixture::AbstractOutputParserTestFixture(const std::wstring &dataPrefix)
  : dataFolder(dataPrefix),
  parserToTest(nullptr)
{
}

AbstractOutputParserTestFixture::~AbstractOutputParserTestFixture()
{
   delete parserToTest;
}

void AbstractOutputParserTestFixture::TestParseInexistentFile()
{
   const bool result = parserToTest->ParseFile(dataFolder + L"nonexistentfile");
   REQUIRE(result == false);
}
/*
void AbstractOutputParserTest::test_ParseFile_Ok()
{
    const bool result = parserToTest->ParseFile(dataFolder + L"dummyDataFile");
    REQUIRE(result == true);
    //REQUIRE(parserToTest->GetFileContent() == L"multi\nline\ncontent\n");
}

void AbstractOutputParserTest::test_WriteFullDescriptionToFile_Ok()
{
    const std::wstring outputFile(L"outputFile");
    const std::wstring contentToWrite(L"my\nmulti\nline\nwritten\ncontent");

    //parserToTest->SetFullDescription(contentToWrite);
    //parserToTest->WriteFullDescriptionToFile(outputFile);

    const std::wstring readContent = FileTools::GetTextFileContent(outputFile);
    REQUIRE(readContent == contentToWrite);
}
*/
