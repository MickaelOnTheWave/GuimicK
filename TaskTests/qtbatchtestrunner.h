#ifndef QTBATCHTESTRUNNER_H
#define QTBATCHTESTRUNNER_H

#include <QObject>
#include <vector>

#include "testunitdata.h"

class QtBatchTestRunner
{
public:
    QtBatchTestRunner(const QString& _applicationName);

    void Run(const std::vector<QObject*>& tests);

    void ShowFullData(void) const;
    void ShowSummarizedData(void) const;

private:
    void ShowTotalLine(const int okCount, const int failCount) const;

    std::vector<TestUnitData> lastResults;
    QString applicationName;
};

#endif // QTBATCHTESTRUNNER_H
