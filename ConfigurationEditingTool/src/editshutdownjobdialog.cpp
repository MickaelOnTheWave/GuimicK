#include "editshutdownjobdialog.h"
#include "ui_editshutdownjobdialog.h"

#include "linuxshutdownjob.h"

EditShutdownJobDialog::EditShutdownJobDialog(AbstractJob* _job) :
   AbstractEditJobDialog(_job),
   ui(new Ui::EditShutdownJobDialog)
{
   ui->setupUi(this);
   UpdateUiFromJob();
}

EditShutdownJobDialog::~EditShutdownJobDialog()
{
   delete ui;
}

void EditShutdownJobDialog::on_okButton_clicked()
{
   UpdateJobFromUi();
   accept();
}

void EditShutdownJobDialog::on_cancelButton_clicked()
{
   reject();
}

void EditShutdownJobDialog::UpdateUiFromJob()
{
   auto shutdownJob = static_cast<LinuxShutdownJob*>(job);
   ui->nameEdit->setText(QString::fromStdWString(job->GetName()));
   ui->timeoutBox->setValue(shutdownJob->GetTimeout());
}

void EditShutdownJobDialog::UpdateJobFromUi()
{
   auto shutdownJob = static_cast<LinuxShutdownJob*>(job);
   job->SetName(ui->nameEdit->text().toStdWString());
   shutdownJob->SetTimeout(ui->timeoutBox->value());
}
