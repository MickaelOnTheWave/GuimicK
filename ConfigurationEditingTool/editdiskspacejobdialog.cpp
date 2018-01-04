#include "editdiskspacejobdialog.h"
#include "ui_editdiskspacejobdialog.h"

#include "linuxfreespacecheckjob.h"

EditDiskSpaceJobDialog::EditDiskSpaceJobDialog(AbstractJob* _job) :
   AbstractEditJobDialog(_job),
   ui(new Ui::EditDiskSpaceJobDialog)
{
   ui->setupUi(this);
   UpdateUiFromJob();
}

EditDiskSpaceJobDialog::~EditDiskSpaceJobDialog()
{
   delete ui;
}

void EditDiskSpaceJobDialog::on_buttonBox_accepted()
{
   UpdateJobFromUi();
   accept();
}

void EditDiskSpaceJobDialog::on_buttonBox_rejected()
{
   reject();
}

void EditDiskSpaceJobDialog::UpdateUiFromJob()
{
   auto diskSpaceJob = static_cast<LinuxFreeSpaceCheckJob*>(job);
   ui->jobNameEdit->setText(diskSpaceJob->GetName().c_str());
   ui->driveEdit->setText(diskSpaceJob->GetDrive().c_str());
}

void EditDiskSpaceJobDialog::UpdateJobFromUi()
{
   auto diskSpaceJob = static_cast<LinuxFreeSpaceCheckJob*>(job);
   diskSpaceJob->SetName(ui->jobNameEdit->text().toStdString());
   diskSpaceJob->SetDrive(ui->driveEdit->text().toStdString());
   diskSpaceJob->SetTargetToLocal(true);
}
