#ifndef TASKTOOLSETTINGSDIALOG_H
#define TASKTOOLSETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
   class TaskToolSettingsDialog;
}

class TaskToolSettingsDialog : public QDialog
{
   Q_OBJECT

public:
   explicit TaskToolSettingsDialog(QWidget *parent = 0);
   ~TaskToolSettingsDialog();

   QString GetTaskToolExecutablePath() const;
   void SetTaskToolExecutable(const QString& value);

   QString GetConfigurationTempPath() const;
   void SetConfigurationTempPath(const QString& value);

private:
   void InitializePathWidgets();

   Ui::TaskToolSettingsDialog *ui;
};

#endif // TASKTOOLSETTINGSDIALOG_H
