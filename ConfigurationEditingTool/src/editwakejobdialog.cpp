#include "editwakejobdialog.h"
#include "ui_editwakejobdialog.h"
#include "abstractwakejob.h"

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
   auto wakeJob = static_cast<AbstractWakeJob*>(job);
   ui->nameEdit->setText(QString::fromStdWString(job->GetName()));
   ui->retriesBox->setValue(wakeJob->GetMaxRetries());
   ui->timeoutBox->setValue(wakeJob->GetTimeout());
}

void EditWakeJobDialog::UpdateJobFromUi()
{
   auto wakeJob = static_cast<AbstractWakeJob*>(job);
   job->SetName(ui->nameEdit->text().toStdWString());
   wakeJob->SetMaxRetries(ui->retriesBox->value());
   wakeJob->SetTimeout(ui->timeoutBox->value());
}
