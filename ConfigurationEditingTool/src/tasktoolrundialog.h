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

private slots:
   void on_runButton_clicked();

private:
   std::string CreateTaskToolCommand() const;
   QString CreateChdirErrorMessage() const;
   QString CreateExecutionErrorMessage(const int returnValue) const;
   void SetupReportDisplay();

   Ui::TaskToolRunDialog *ui;

   QString runPath = "";
   QString configurationFile = "";
   QString taskToolExecutable = "";
   QString reportFile = "";
};

#endif // TASKTOOLRUNDIALOG_H
