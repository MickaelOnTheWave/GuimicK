#ifndef TASKFEATURETEST_H
#define TASKFEATURETEST_H

#include <QObject>
#include <QStringList>

#include "configuration.h"

class TaskFeatureTest : public QObject
{
    Q_OBJECT

public:
    TaskFeatureTest();

private Q_SLOTS:
    void init();
    void cleanup();

    void testRun_data();
    void testRun();

private:
    void CopyDataFolders();
    QStringList GetAttachmentFiles(const std::string& folder);

    void ReadConfiguration(Configuration& configuration);
    void CheckReport(const std::string& reportContent);
    void CheckAttachments(WorkResultData* results);

    std::string currentTestCaseName = "";
    std::string currentTestCaseFolder = "";
    QStringList currentDataFolders;
};

#endif // TASKFEATURETEST_H
