#ifndef ABSTRACTOUTPUTPARSERTEST_H
#define ABSTRACTOUTPUTPARSERTEST_H

#include "qttestsuite.h"

#include <QString>
#include <QtTest>

class AbstractOutputParserTest : public QtTestSuite
{
    Q_OBJECT

public:
    AbstractOutputParserTest(const std::string& dataPrefix);

private Q_SLOTS:
    void test_ParseFile_InvalidFile();
    void test_ParseFile_Ok();
    void test_WriteFullDescriptionToFile_Ok();

private:
    QString GetContentFromFile(const std::string& file);
};

#endif // ABSTRACTOUTPUTPARSERTEST_H
