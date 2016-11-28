#ifndef TASKFEATURETEST_H
#define TASKFEATURETEST_H

#include <QObject>

class TaskFeatureTest : public QObject
{
    Q_OBJECT

public:
    TaskFeatureTest();

private Q_SLOTS:
    void init();
    void cleanup();

    void testGitBackup_data();
    void testGitBackup();

private:
    void CheckAttachments(const std::vector<std::pair<std::string,std::string> >& attachments);

};

#endif // TASKFEATURETEST_H
