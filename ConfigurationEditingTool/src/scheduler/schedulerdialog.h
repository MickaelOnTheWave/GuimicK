#ifndef SCHEDULERDIALOG_H
#define SCHEDULERDIALOG_H

#include <map>
#include <QCheckBox>
#include <QDialog>

#include "abstractscheduler.h"
#include "scheduledata.h"

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
   void SetTaskToolExecutable(const QString& value);

private slots:
   void on_noScheduleButton_clicked();

   void on_scheduleButton_clicked();

   void on_dailyButton_clicked();

   void on_weeklyButton_clicked();

   void on_monthlyButton_clicked();

   void on_buttonBox_accepted();

private:
   void CreateDaysOfMonthControls();
   void InitializePathWidget();

   void InitializeScheduler();
   void CreateScheduler();
   void ReadSchedulerData();
   void WriteSchedulerData();

   void UpdateUiFromScheduleData(ScheduleData* data);
   ScheduleData* CreateScheduleDataFromUi() const;

   ScheduleData* CreateDailyScheduleDataFromUi() const;
   ScheduleData* CreateWeeklyScheduleDataFromUi() const;
   ScheduleData* CreateMonthlyScheduleDataFromUi() const;

   void SetSchedulerCommandData();

   void UpdateWeekDaysCheckboxes(const std::vector<int>& days);
   void UncheckAllWeekDaysCheckboxes();

   void UpdateMonthDaysCheckboxes(const std::vector<int>& days);

   Ui::SchedulerDialog *ui;
   AbstractScheduler* scheduler = nullptr;
   std::map<QCheckBox*,int> monthlyCheckboxes;
   QString taskToolExecutable = "";
};

#endif // SCHEDULERDIALOG_H
