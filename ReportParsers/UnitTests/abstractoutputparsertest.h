#ifndef ABSTRACTOUTPUTPARSERTEST_H
#define ABSTRACTOUTPUTPARSERTEST_H

#include <QObject>
#include <QString>
#include <QtTest>

class AbstractOutputParserTest : public QObject
{
    Q_OBJECT

public:
    AbstractOutputParserTest();

private Q_SLOTS:
    void test_ParseFile_InvalidFile();
    void test_ParseFile_Ok();
    void test_WriteFullDescriptionToFile_Ok();

private:
    QString GetContentFromFile(const std::string& file);
};

#endif // ABSTRACTOUTPUTPARSERTEST_H
