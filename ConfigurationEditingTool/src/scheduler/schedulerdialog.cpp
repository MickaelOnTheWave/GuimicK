#include "schedulerdialog.h"
#include "ui_schedulerdialog.h"

#include <QMessageBox>
#include "windowsscheduler.h"

SchedulerDialog::SchedulerDialog(QWidget *parent) :
   QDialog(parent),
   ui(new Ui::SchedulerDialog)
{
   ui->setupUi(this);
   on_noScheduleButton_clicked();
   on_dailyButton_clicked();

   InitializePathWidget();
   InitializeScheduler();
}

SchedulerDialog::~SchedulerDialog()
{
   delete ui;
}

void SchedulerDialog::SetConfigurationFile(const QString& file)
{
   configurationFile = file;
   ui->fileSelectionDialog->SetPath(configurationFile);
}

void SchedulerDialog::on_noScheduleButton_clicked()
{
   ui->configurationGroupBox->setEnabled(false);
}

void SchedulerDialog::on_scheduleButton_clicked()
{
   ui->configurationGroupBox->setEnabled(true);
}

void SchedulerDialog::on_dailyButton_clicked()
{
   ui->configurationStackedWidget->setCurrentIndex(0);
}

void SchedulerDialog::on_weeklyButton_clicked()
{
   ui->configurationStackedWidget->setCurrentIndex(1);
}

void SchedulerDialog::on_monthlyButton_clicked()
{
   ui->configurationStackedWidget->setCurrentIndex(2);
}

void SchedulerDialog::InitializePathWidget()
{
   const QString fileFilter = "Configuration file (*)";
   ui->fileSelectionDialog->InitializeAsExistingFile(
            "Configuration file",
            "Select configuration to use for Scheduled Runs",
            "",
            fileFilter);
}

void SchedulerDialog::InitializeScheduler()
{
   CreateScheduler();
   if (scheduler)
      ReadSchedulerData();
}

void SchedulerDialog::CreateScheduler()
{
#ifdef _WIN32
   scheduler = new WindowsScheduler();
#else
   scheduler = nullptr;
#endif
}

void SchedulerDialog::ReadSchedulerData()
{
   ScheduleTarget scheduleData;
   const bool ok = scheduler->Read(scheduleData);
   if (ok)
      UpdateUiFromScheduleData(scheduleData);
   else
      QMessageBox::warning(this, "Error", "System scheduler data could not be read");
}

void SchedulerDialog::WriteSchedulerData()
{
   const bool ok = scheduler->Write(GetScheduleDataFromUi());
   if (!ok)
      QMessageBox::warning(this, "Error", "System scheduler data could not be modified");
}

void SchedulerDialog::UpdateUiFromScheduleData(const ScheduleTarget& data)
{
   switch (data.type)
   {
      case ScheduleTarget::Type::Daily :
      {
         ui->dailyButton->setChecked(true);
         break;
      }
      case ScheduleTarget::Type::Weekly :
      {
         ui->weeklyButton->setChecked(true);
         break;
      }
      case ScheduleTarget::Type::Monthly :
      {
         ui->monthlyButton->setChecked(true);
         break;
      }
      default:
      break;
   }
}

ScheduleTarget SchedulerDialog::GetScheduleDataFromUi() const
{
   ScheduleTarget data;
   if (ui->dailyButton->isChecked())
      data.type = ScheduleTarget::Type::Daily;
   else if (ui->weeklyButton->isChecked())
      data.type = ScheduleTarget::Type::Weekly;
   else if (ui->monthlyButton->isChecked())
      data.type = ScheduleTarget::Type::Monthly;
   return data;
}

void SchedulerDialog::on_buttonBox_accepted()
{
   if (scheduler)
      WriteSchedulerData();
   accept();
}
