#include "editconsolejobdialog.h"
#include "ui_editconsolejobdialog.h"

#include "userconsolejob.h"

EditConsoleJobDialog::EditConsoleJobDialog(AbstractJob* _job) :
   AbstractEditJobDialog(_job),
   ui(new Ui::EditConsoleJobDialog)
{
   ui->setupUi(this);
   UpdateUiFromJob();
}

EditConsoleJobDialog::~EditConsoleJobDialog()
{
   delete ui;
}

void EditConsoleJobDialog::on_okButton_clicked()
{
   UpdateJobFromUi();
   accept();
}

void EditConsoleJobDialog::on_cancelButton_clicked()
{
   reject();
}

void EditConsoleJobDialog::UpdateUiFromJob()
{
   auto consoleJob = static_cast<UserConsoleJob*>(job);
   ui->nameEdit->setText(job->GetName().c_str());
   ui->commandEdit->setText(consoleJob->GetCommand().c_str());
   ui->parametersEdit->setText(consoleJob->GetCommandParameters().c_str());
   ui->expectedCodeSpinBox->setValue(consoleJob->GetExpectedReturnCode());
   ui->expectedOutputTextEdit->setPlainText(consoleJob->GetExpectedOutput().c_str());
   //ui->successTypeBox->setCurrentIndex(consoleJob->ge);
}

void EditConsoleJobDialog::UpdateJobFromUi()
{
   auto consoleJob = static_cast<UserConsoleJob*>(job);
   job->SetName(ui->nameEdit->text().toStdString());
   consoleJob->SetCommand(ui->commandEdit->text().toStdString());
}

void EditConsoleJobDialog::on_successTypeBox_activated(const QString &arg1)
{
}

void EditConsoleJobDialog::on_successTypeBox_currentIndexChanged(int index)
{
   ui->successTypeWidget->setCurrentIndex(index);
}
