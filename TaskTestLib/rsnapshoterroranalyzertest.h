#ifndef RSNAPSHOTERRORANALYZERTEST_H
#define RSNAPSHOTERRORANALYZERTEST_H

#include <QObject>

class RsnapshotErrorAnalyzerTest : public QObject
{
    Q_OBJECT
public:
     RsnapshotErrorAnalyzerTest();

private Q_SLOTS:
    void init();
    void cleanup();

    void testOutput();
    void testOutput_data();

private:
};

#endif // RSNAPSHOTERRORANALYZERTEST_H
