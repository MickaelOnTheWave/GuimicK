#ifndef ABSTRACTOUTPUTPARSERTESTFIXTURE_H
#define ABSTRACTOUTPUTPARSERTESTFIXTURE_H

#include <string>
#include "abstractoutputparser.h"

class AbstractOutputParserTestFixture
{
public:
   AbstractOutputParserTestFixture() = default;
   virtual ~AbstractOutputParserTestFixture();

   void SetParser(AbstractOutputParser* parser);

   void TestParseInexistentFile();
   //void test_ParseFile_Ok();
   //void test_WriteFullDescriptionToFile_Ok();

protected:
   std::wstring dataFolder;
   AbstractOutputParser* parserToTest = nullptr;
};

#endif // ABSTRACTOUTPUTPARSERTESTFIXTURE_H
