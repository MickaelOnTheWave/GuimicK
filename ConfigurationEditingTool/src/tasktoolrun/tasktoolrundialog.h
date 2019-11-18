#ifndef TASKTOOLRUNDIALOG_H
#define TASKTOOLRUNDIALOG_H

#include <QDialog>
#include <QThread>

#include "adminrightswarning.h"
#include "abstracttasktoolrunner.h"
#include "tasktooldata.h"

namespace Ui {
   class TaskToolRunDialog;
}

class TaskToolRunDialog : public QDialog
{
   Q_OBJECT

public:
   explicit TaskToolRunDialog(QWidget *parent, const bool showAdminWarning);
   ~TaskToolRunDialog();

   void SetConfigurationData(const TaskToolConfigurationData& data);
   void SetReportFile(const QString& value);
   void SetReportFolder(const QString& value);
   void SetReportType(const std::wstring& value);

private slots:
   void on_runButton_clicked();
   void OnFinishedRunningTaskTool();

private:
   std::wstring CreateTaskToolCommand() const;
   std::wstring CreateWindowsTaskToolCommand() const;
   std::wstring CreateLinuxTaskToolCommand() const;

   QString CreateChdirErrorMessage() const;
   QString CreateExecutionErrorMessage(const int returnValue,
                                       const std::wstring& output) const;
   QString CreateConfigurationFileErrorMessage() const;
   QString CreateGenericExecutionErrorMessage(const int returnValue,
                                       const std::wstring& output) const;
   void SetupReportDisplay();

   void SetupReportFilesDisplay();
   void ClearFileBox();
   QStringList FindReportFiles() const;
   void AddFileLink(const QString& file);
   QString BuildTextLabel(const QString& file) const;

   void CleanPreviousReport();

   void AddAdminRightsWarning();

   void InitializeThreadedTaskToolRun();
   void StartTaskTool();

   void SetUiWaitState();
   void UnfreezeUi();

   void UpdateTaskToolUiWithRunResult();

   void UpdateTaskToolUiWithResults(
         const bool success,
         const QString& output
   );

   AbstractTaskToolRunner* CreateRunner();

   Ui::TaskToolRunDialog *ui;
   AdminRightsWarning* warningWidget;



   TaskToolConfigurationData configData;
   QString reportFile = "";
   QString reportFolder = "";
   std::wstring reportType = L"";

   std::wstring currentDirectory = L"";
   QThread taskToolThread;
   AbstractTaskToolRunner* taskToolRunner = nullptr;
};

#endif // TASKTOOLRUNDIALOG_H
