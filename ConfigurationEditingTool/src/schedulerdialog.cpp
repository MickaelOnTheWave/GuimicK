#include "schedulerdialog.h"
#include "ui_schedulerdialog.h"

SchedulerDialog::SchedulerDialog(QWidget *parent) :
   QDialog(parent),
   ui(new Ui::SchedulerDialog)
{
   ui->setupUi(this);
   on_noScheduleButton_clicked();
   on_dailyButton_clicked();
}

SchedulerDialog::~SchedulerDialog()
{
   delete ui;
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
