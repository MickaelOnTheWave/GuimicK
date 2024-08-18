#include "filebackupparsertestfixture.h"

#include <catch2/catch.hpp>
#include "abstractfilebackupparser.h"

using namespace std;

void FileBackupParserTestFixture::TestParseOk(const std::wstring &file, const FileBackupReport &expectedData)
{
   const bool returnValue = parserToTest->ParseFile(dataFolder + file);
   REQUIRE(returnValue == true);

   auto typedParser = dynamic_cast<AbstractFileBackupParser*>(parserToTest);
   REQUIRE(typedParser != nullptr);

   FileBackupReport returnedData;
   typedParser->GetReport(returnedData);
   REQUIRE(returnedData == expectedData);
}
