#ifndef DFCOMMANDPARSERTEST_H
#define DFCOMMANDPARSERTEST_H

#include <QObject>

#include "dfcommandparser.h"

class DfCommandParserTest : public QObject
{
    Q_OBJECT

public:
    DfCommandParserTest();

private Q_SLOTS:
    void init();
    void cleanup();

    void testParse_data();
    void testParse();

private:
    void CheckDataIsTheSame(const std::vector<Drive>& data,
                            const QStringList& expectedData,
                            std::function<std::string(Drive)> propertyGetter);
};

#endif // DFCOMMANDPARSERTEST_H
