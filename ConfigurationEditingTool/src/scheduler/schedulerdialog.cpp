#include "schedulerdialog.h"
#include "ui_schedulerdialog.h"

#include <QGridLayout>
#include <QMessageBox>
#include <QPushButton>

#ifdef _WIN32
   #include "windowsscheduler.h"
#endif

SchedulerDialog::SchedulerDialog(QWidget *parent) :
   QDialog(parent),
   ui(new Ui::SchedulerDialog)
{
   ui->setupUi(this);
   setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
   ConnectDaysOfWeekControls();
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
   ui->fileSelectionDialog->SetPath(file);
}

void SchedulerDialog::SetTaskToolExecutable(const QString& value)
{
   taskToolExecutable = value;
}

void SchedulerDialog::on_noScheduleButton_clicked()
{
   ui->configurationGroupBox->setEnabled(false);
   SetOkButtonEnabled(true);
}

void SchedulerDialog::on_scheduleButton_clicked()
{
   ui->configurationGroupBox->setEnabled(true);

   if (ui->dailyButton->isChecked())
      SetOkButtonEnabled(true);
   else if (ui->weeklyButton->isChecked())
      SetOkButtonEnabled(IsOneWeeklyCheckboxChecked());
   else if (ui->monthlyButton->isChecked())
      SetOkButtonEnabled(IsOneMonthlyCheckboxChecked());
}

void SchedulerDialog::on_dailyButton_clicked()
{
   ui->configurationStackedWidget->setCurrentIndex(0);
   SetOkButtonEnabled(true);
}

void SchedulerDialog::on_weeklyButton_clicked()
{
   ui->configurationStackedWidget->setCurrentIndex(1);
   SetOkButtonEnabled(IsOneWeeklyCheckboxChecked());
}

void SchedulerDialog::on_monthlyButton_clicked()
{
   ui->configurationStackedWidget->setCurrentIndex(2);
   SetOkButtonEnabled(IsOneMonthlyCheckboxChecked());
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
   ScheduleData* scheduleData = nullptr;
   const bool ok = scheduler->Read(&scheduleData);
   if (ok)
      UpdateUiFromScheduleData(scheduleData);
   else
      QMessageBox::warning(this, "Error", "System scheduler data could not be read");
}

void SchedulerDialog::WriteSchedulerData()
{
   SetSchedulerCommandData();
   const bool ok = scheduler->Write(CreateScheduleDataFromUi());
   if (!ok)
   {
      QString errorMessage = "Error trying to update Scheduler data :\n\n";
      errorMessage += scheduler->GetLastError();
      QMessageBox::warning(this, "System scheduler Update", errorMessage);
   }
}

void SchedulerDialog::UpdateUiFromScheduleData(ScheduleData* scheduleData)
{
   if (auto monthlyData = dynamic_cast<ScheduleMonthlyData*>(scheduleData))
   {
      ui->scheduleButton->setChecked(true);
      on_scheduleButton_clicked();

      ui->monthlyButton->setChecked(true);
      on_monthlyButton_clicked();

      ui->scheduleTimeEdit->setTime(scheduleData->GetTime());
      std::vector<int> days = monthlyData->GetDaysIndices();
      UpdateMonthDaysCheckboxes(days);
   }
   else if (dynamic_cast<ScheduleDailyData*>(scheduleData))
   {
      ui->scheduleButton->setChecked(true);
      on_scheduleButton_clicked();

      ui->dailyButton->setChecked(true);
      on_dailyButton_clicked();

      ui->scheduleTimeEdit->setTime(scheduleData->GetTime());
   }
   else if (auto weeklyData = dynamic_cast<ScheduleWeeklyData*>(scheduleData))
   {
      ui->scheduleButton->setChecked(true);
      on_scheduleButton_clicked();

      ui->weeklyButton->setChecked(true);
      on_weeklyButton_clicked();

      ui->scheduleTimeEdit->setTime(scheduleData->GetTime());
      std::vector<int> days = weeklyData->GetDaysIndices();
      UpdateWeekDaysCheckboxes(days);
   }
   else
      on_noScheduleButton_clicked();
}

ScheduleData* SchedulerDialog::CreateScheduleDataFromUi() const
{
   if (ui->scheduleButton->isChecked())
   {
      if (ui->dailyButton->isChecked())
         return CreateDailyScheduleDataFromUi();
      else if (ui->weeklyButton->isChecked())
         return CreateWeeklyScheduleDataFromUi();
      else if (ui->monthlyButton->isChecked())
         return CreateMonthlyScheduleDataFromUi();
   }
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
   if (ui->wednesdayBox->isChecked())
      scheduleData->AddDayIndex(3);
   if (ui->thursdayBox->isChecked())
      scheduleData->AddDayIndex(4);
   if (ui->fridayBox->isChecked())
      scheduleData->AddDayIndex(5);
   if (ui->saturdayBox->isChecked())
      scheduleData->AddDayIndex(6);
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

void SchedulerDialog::SetSchedulerCommandData()
{
   const QString command = QString("\"") + taskToolExecutable + "\"";
   const QString arguments = QString("--conffile \"") + ui->fileSelectionDialog->GetPath() + "\"";

   scheduler->SetCommandToRun(command);
   scheduler->SetCommandArguments(arguments);
}

void SchedulerDialog::UpdateWeekDaysCheckboxes(const std::vector<int>& days)
{
   UncheckAllWeekDaysCheckboxes();
   for (const int day : days)
   {
      if (day == 0)
         ui->sundayBox->setChecked(true);
      else if (day == 1)
         ui->mondayBox->setChecked(true);
      else if (day == 2)
         ui->tuesdayBox->setChecked(true);
      else if (day == 3)
         ui->wednesdayBox->setChecked(true);
      else if (day == 4)
         ui->thursdayBox->setChecked(true);
      else if (day == 5)
         ui->fridayBox->setChecked(true);
      else if (day == 6)
         ui->saturdayBox->setChecked(true);
   }
}

void SchedulerDialog::UncheckAllWeekDaysCheckboxes()
{
   ui->sundayBox->setChecked(false);
   ui->mondayBox->setChecked(false);
   ui->tuesdayBox->setChecked(false);
   ui->wednesdayBox->setChecked(false);
   ui->thursdayBox->setChecked(false);
   ui->fridayBox->setChecked(false);
   ui->saturdayBox->setChecked(false);
}

void SchedulerDialog::UpdateMonthDaysCheckboxes(const std::vector<int>& days)
{
   int i = 0;
   const QObjectList checkboxList = ui->montlyPage->children();
   for (const auto object : checkboxList)
   {
      auto checkbox = dynamic_cast<QCheckBox*>(object);
      if (checkbox)
      {
         const bool isDayPresent = std::find(days.begin(), days.end(), i) != days.end();
         checkbox->setChecked(isDayPresent);
         ++i;
      }
   }
}

void SchedulerDialog::SetOkButtonEnabled(const bool value)
{
   QPushButton* okButton = ui->closeButtonBox->button(QDialogButtonBox::Ok);
   okButton->setEnabled(value);
}

bool SchedulerDialog::IsOneWeeklyCheckboxChecked() const
{
   if (ui->sundayBox->isChecked())
      return true;
   else if (ui->mondayBox->isChecked())
      return true;
   else if (ui->tuesdayBox->isChecked())
      return true;
   else if (ui->wednesdayBox->isChecked())
      return true;
   else if (ui->thursdayBox->isChecked())
      return true;
   else if (ui->fridayBox->isChecked())
      return true;
   else if (ui->saturdayBox->isChecked())
      return true;
   else
      return false;
}

bool SchedulerDialog::IsOneMonthlyCheckboxChecked() const
{
   const QObjectList checkboxList = ui->montlyPage->children();
   for (const auto object : checkboxList)
   {
      auto checkbox = dynamic_cast<QCheckBox*>(object);
      if (checkbox && checkbox->isChecked())
         return true;
   }

   return false;
}

void SchedulerDialog::on_closeButtonBox_accepted()
{
   if (scheduler)
      WriteSchedulerData();
   accept();
}

void SchedulerDialog::onWeeklyCheckboxCheck(bool value)
{
   if (value)
      SetOkButtonEnabled(true);
   else
      SetOkButtonEnabled(IsOneWeeklyCheckboxChecked());
}

void SchedulerDialog::onMonthlyCheckboxCheck(bool value)
{
   if (value)
      SetOkButtonEnabled(true);
   else
      SetOkButtonEnabled(IsOneMonthlyCheckboxChecked());
}

void SchedulerDialog::ConnectDaysOfWeekControls()
{
   connect(ui->sundayBox, SIGNAL(clicked(bool)), this, SLOT(onWeeklyCheckboxCheck(bool)));
   connect(ui->mondayBox, SIGNAL(clicked(bool)), this, SLOT(onWeeklyCheckboxCheck(bool)));
   connect(ui->tuesdayBox, SIGNAL(clicked(bool)), this, SLOT(onWeeklyCheckboxCheck(bool)));
   connect(ui->wednesdayBox, SIGNAL(clicked(bool)), this, SLOT(onWeeklyCheckboxCheck(bool)));
   connect(ui->thursdayBox, SIGNAL(clicked(bool)), this, SLOT(onWeeklyCheckboxCheck(bool)));
   connect(ui->fridayBox, SIGNAL(clicked(bool)), this, SLOT(onWeeklyCheckboxCheck(bool)));
   connect(ui->saturdayBox, SIGNAL(clicked(bool)), this, SLOT(onWeeklyCheckboxCheck(bool)));
}

void SchedulerDialog::CreateDaysOfMonthControls()
{
   auto gridLayout = new QGridLayout();

   for (int i=0; i<31; ++i)
   {
      const int row = i%8;
      const int column = i/8;

      auto checkbox = new QCheckBox(QString::number(i+1), ui->montlyPage);
      connect(checkbox, SIGNAL(clicked(bool)), this, SLOT(onMonthlyCheckboxCheck(bool)));
      gridLayout->addWidget(checkbox, row, column);
      monthlyCheckboxes.insert(std::make_pair(checkbox, i));
   }

   ui->montlyPage->layout()->addItem(gridLayout);
}
