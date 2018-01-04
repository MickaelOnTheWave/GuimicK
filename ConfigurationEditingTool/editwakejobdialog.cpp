#include "editwakejobdialog.h"
#include "ui_editwakejobdialog.h"
#include "wakejob.h"

EditWakeJobDialog::EditWakeJobDialog(AbstractJob* _job) :
   AbstractEditJobDialog(_job),
   ui(new Ui::EditWakeJobDialog)
{
   ui->setupUi(this);
   UpdateUiFromJob();
   connect(ui->okButton, SIGNAL(pressed()), this, SLOT(OnCloseAccepting()));
   connect(ui->cancelButton, SIGNAL(pressed()), this, SLOT(OnCloseRejecting()));
}

EditWakeJobDialog::~EditWakeJobDialog()
{
   delete ui;
}

void EditWakeJobDialog::UpdateUiFromJob()
{
   auto wakeJob = static_cast<WakeJob*>(job);
   ui->nameEdit->setText(job->GetName().c_str());
   ui->retriesBox->setValue(wakeJob->GetMaxRetries());
   ui->timeoutBox->setValue(wakeJob->GetTimeout());
}

void EditWakeJobDialog::UpdateJobFromUi()
{
   auto wakeJob = static_cast<WakeJob*>(job);
   job->SetName(ui->nameEdit->text().toStdString());
   wakeJob->SetMaxRetries(ui->retriesBox->value());
   wakeJob->SetTimeout(ui->timeoutBox->value());
}
