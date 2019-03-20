#ifndef TASKTOOLRUNDIALOG_H
#define TASKTOOLRUNDIALOG_H

#include <QDialog>

namespace Ui {
   class TaskToolRunDialog;
}

class TaskToolRunDialog : public QDialog
{
   Q_OBJECT

public:
   explicit TaskToolRunDialog(QWidget *parent = 0);
   ~TaskToolRunDialog();

   void SetRunPath(const QString& value);
   void SetConfigurationFile(const QString& value);
   void SetToolExecutable(const QString& value);
   void SetReportFile(const QString& value);
   void SetReportFolder(const QString& value);
   void SetReportType(const std::string& value);

private slots:
   void on_runButton_clicked();

private:
   std::wstring CreateTaskToolCommand() const;
   QString CreateChdirErrorMessage() const;
   QString CreateExecutionErrorMessage(const int returnValue,
                                       const std::wstring& output) const;
   void SetupReportDisplay();

   void SetupReportFilesDisplay();
   void ClearFileBox();
   QStringList FindReportFiles() const;
   void AddFileLink(const QString& file);
   QString BuildTextLabel(const QString& file) const;

   void CleanPreviousReport();

   Ui::TaskToolRunDialog *ui;

   QString runPath = "";
   QString configurationFile = "";
   QString taskToolExecutable = "";
   QString reportFile = "";
   QString reportFolder = "";
   std::string reportType = "";
};

#endif // TASKTOOLRUNDIALOG_H
