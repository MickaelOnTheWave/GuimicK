#ifndef TASKFEATURETEST_H
#define TASKFEATURETEST_H

#include <QObject>
#include <QStringList>

#include "configuration.h"

class TaskFeatureTest : public QObject
{
    Q_OBJECT

public:
    TaskFeatureTest(const std::string& _dataFolder, const std::string& _errorFolder);

private Q_SLOTS:
    void init();
    void cleanup();

    void testRun_data();
    void testRun();

private:
    void CheckFinalReport();

    std::vector<std::string> GetAttachmentFiles() const;

    void CopyDataFolders();
    QStringList GetAttachmentFiles(const std::string& folder);

    void ReadConfiguration(Configuration& configuration);
    void CheckReport(const std::string& reportContent);
    void CheckAttachments(const std::vector<std::string>& files);

    void GetAttachmentContents(const QStringList& fileList,
                               std::vector<std::string>& contentList);
    void CheckAttachmentContentsAreEqual(const std::vector<std::string>& contents,
                                         const std::vector<std::string>& expectedContents);
    void WriteAttachment(const std::string& content, const int number);

    std::string currentTestCaseName = "";
    std::string currentTestCaseFolder = "";

    const std::string suiteFolder;
    const std::string errorFolder;
};

#endif // TASKFEATURETEST_H
