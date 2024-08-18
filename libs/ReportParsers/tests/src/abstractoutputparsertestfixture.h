#ifndef ABSTRACTOUTPUTPARSERTESTFIXTURE_H
#define ABSTRACTOUTPUTPARSERTESTFIXTURE_H

#include <string>
#include "abstractoutputparser.h"

class AbstractOutputParserTestFixture
{
public:
   AbstractOutputParserTestFixture(const std::wstring& dataPrefix);
   virtual ~AbstractOutputParserTestFixture();

   void TestParseInexistentFile();
   //void test_ParseFile_Ok();
   //void test_WriteFullDescriptionToFile_Ok();

protected:
   std::wstring dataFolder;
   AbstractOutputParser* parserToTest;
};

#endif // ABSTRACTOUTPUTPARSERTESTFIXTURE_H
