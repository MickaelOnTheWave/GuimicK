#ifndef SCHEDULERDIALOG_H
#define SCHEDULERDIALOG_H

#include <QDialog>

namespace Ui {
   class SchedulerDialog;
}

class SchedulerDialog : public QDialog
{
   Q_OBJECT

public:
   explicit SchedulerDialog(QWidget *parent = 0);
   ~SchedulerDialog();

   void SetConfigurationFile(const QString& file);

private slots:
   void on_noScheduleButton_clicked();

   void on_scheduleButton_clicked();

   void on_dailyButton_clicked();

   void on_weeklyButton_clicked();

   void on_monthlyButton_clicked();

private:
   void InitializePathWidget();

   Ui::SchedulerDialog *ui;
   QString configurationFile = "";
};

#endif // SCHEDULERDIALOG_H
