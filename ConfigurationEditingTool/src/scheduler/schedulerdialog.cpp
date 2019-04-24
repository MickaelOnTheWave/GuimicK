#include "schedulerdialog.h"
#include "ui_schedulerdialog.h"

#include <QGridLayout>
#include <QMessageBox>

#ifdef _WIN32
   #include "windowsscheduler.h"
#endif

SchedulerDialog::SchedulerDialog(QWidget *parent) :
   QDialog(parent),
   ui(new Ui::SchedulerDialog)
{
   ui->setupUi(this);
   CreateDaysOfMonthControls();
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
   const bool ok = scheduler->Write(CreateScheduleDataFromUi());
   if (!ok)
   {
      QString errorMessage = "Error trying to update Scheduler data :\n\n";
      errorMessage += scheduler->GetLastError();
      QMessageBox::warning(this, "System scheduler Update", errorMessage);
   }
}

void SchedulerDialog::UpdateUiFromScheduleData(const ScheduleTarget& scheduleData)
{
   switch (scheduleData.type)
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

ScheduleData* SchedulerDialog::CreateScheduleDataFromUi() const
{
   if (ui->dailyButton->isChecked())
      return CreateDailyScheduleDataFromUi();
   else if (ui->weeklyButton->isChecked())
      return CreateWeeklyScheduleDataFromUi();
   else if (ui->monthlyButton->isChecked())
      return CreateMonthlyScheduleDataFromUi();
   else
      return nullptr;
}

ScheduleData* SchedulerDialog::CreateDailyScheduleDataFromUi() const
{
   auto scheduleData = new ScheduleDailyData();
   scheduleData->SetTime(ui->scheduleTimeEdit->time());
   return scheduleData;
}

ScheduleData* SchedulerDialog::CreateWeeklyScheduleDataFromUi() const
{
   auto scheduleData = new ScheduleWeeklyData();
   scheduleData->SetTime(ui->scheduleTimeEdit->time());

   if (ui->sundayBox->isChecked())
      scheduleData->AddDayIndex(0);
   if (ui->mondayBox->isChecked())
      scheduleData->AddDayIndex(1);
   if (ui->tuesdayBox->isChecked())
      scheduleData->AddDayIndex(2);
   return scheduleData;
}

ScheduleData* SchedulerDialog::CreateMonthlyScheduleDataFromUi() const
{
   auto scheduleData = new ScheduleMonthlyData();
   scheduleData->SetTime(ui->scheduleTimeEdit->time());

   for (auto checkbox : monthlyCheckboxes)
   {
      if (checkbox.first->isChecked())
         scheduleData->AddDayIndex(checkbox.second);
   }

   return scheduleData;
}

void SchedulerDialog::on_buttonBox_accepted()
{
   if (scheduler)
      WriteSchedulerData();
   accept();
}

void SchedulerDialog::CreateDaysOfMonthControls()
{
   auto gridLayout = new QGridLayout();

   for (int i=0; i<31; ++i)
   {
      const int row = i%8;
      const int column = i/8;

      auto checkbox = new QCheckBox(QString::number(i+1), ui->montlyPage);
      gridLayout->addWidget(checkbox, row, column);
      monthlyCheckboxes.insert(std::make_pair(checkbox, i));
   }

   ui->montlyPage->layout()->addItem(gridLayout);
}
