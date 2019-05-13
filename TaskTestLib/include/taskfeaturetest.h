#ifndef TASKFEATURETEST_H
#define TASKFEATURETEST_H

#include <QObject>
#include <QStringList>

//#include "serverconfiguration.h"
#include "taskmanagerconfiguration.h"

class TaskFeatureTest : public QObject
{
    Q_OBJECT

public:
    TaskFeatureTest(const std::wstring& _dataFolder, const std::wstring& _errorFolder);

private Q_SLOTS:
    void init();
    void cleanup();

    void testRun_data();
    void testRun();

private:

    using StdStringVector = std::vector<std::wstring>;

    void CheckFinalReport();

    StdStringVector GetAttachmentFiles() const;

    void CopyDataFolders();
    QStringList GetAttachmentFiles(const std::wstring& folder);

    void ReadConfiguration(TaskManagerConfiguration& configuration);
    void CheckReport(const std::wstring& reportContent);
    void CheckAttachments(const StdStringVector& files);

    void GetAttachmentContents(const QStringList& fileList,
                               StdStringVector& contentList);
    void CheckAttachmentContentsAreEqual(const StdStringVector& contents,
                                         const StdStringVector& expectedContents);
    void WriteAttachment(const std::wstring& content, const int number);

    std::wstring currentTestCaseName = L"";
    std::wstring currentTestCaseFolder = L"";

    const std::wstring suiteFolder;
    const std::wstring errorFolder;
};

#endif // TASKFEATURETEST_H
