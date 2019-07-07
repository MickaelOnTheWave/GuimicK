#ifndef TASKTOOLRUNDIALOG_H
#define TASKTOOLRUNDIALOG_H

#include <QDialog>
#include "adminrightswarning.h"

namespace Ui {
   class TaskToolRunDialog;
}

class TaskToolRunDialog : public QDialog
{
   Q_OBJECT

public:
   explicit TaskToolRunDialog(QWidget *parent, const bool showAdminWarning);
   ~TaskToolRunDialog();

   void SetRunPath(const QString& value);
   void SetConfigurationFile(const QString& value);
   void SetToolExecutable(const QString& value);
   void SetReportFile(const QString& value);
   void SetReportFolder(const QString& value);
   void SetReportType(const std::wstring& value);

private slots:
   void on_runButton_clicked();

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

   Ui::TaskToolRunDialog *ui;
   AdminRightsWarning* warningWidget;



   QString runPath = "";
   QString configurationFile = "";
   QString taskToolExecutable = "";
   QString reportFile = "";
   QString reportFolder = "";
   std::wstring reportType = L"";
};

#endif // TASKTOOLRUNDIALOG_H
