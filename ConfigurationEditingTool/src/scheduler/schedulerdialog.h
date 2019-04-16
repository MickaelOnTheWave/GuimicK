#ifndef SCHEDULERDIALOG_H
#define SCHEDULERDIALOG_H

#include <QDialog>
#include "abstractscheduler.h"

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

   void on_buttonBox_accepted();

private:
   void InitializePathWidget();

   void InitializeScheduler();
   void CreateScheduler();
   void ReadSchedulerData();
   void WriteSchedulerData();

   void UpdateUiFromScheduleData(const ScheduleTarget& data);
   ScheduleTarget GetScheduleDataFromUi() const;

   Ui::SchedulerDialog *ui;
   QString configurationFile = "";
   AbstractScheduler* scheduler = nullptr;
};

#endif // SCHEDULERDIALOG_H
