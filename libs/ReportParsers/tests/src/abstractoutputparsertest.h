#ifndef ABSTRACTOUTPUTPARSERTEST_H
#define ABSTRACTOUTPUTPARSERTEST_H

#include <string>
#include "abstractoutputparser.h"

class AbstractOutputParserTest
{
public:
   AbstractOutputParserTest(const std::wstring& dataPrefix);
   virtual ~AbstractOutputParserTest();

   void TestParseFile_InexistentFile();
   //void test_ParseFile_Ok();
   //void test_WriteFullDescriptionToFile_Ok();

protected:
   std::wstring dataFolder;
   AbstractOutputParser* parserToTest;
};

#endif // ABSTRACTOUTPUTPARSERTEST_H
