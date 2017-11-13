#include "editconsolejobdialog.h"
#include "ui_editconsolejobdialog.h"

#include "sshconsolejob.h"
#include "userconsolejob.h"

using namespace std;

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
   UpdateUiFromJob_Basic();
   UpdateUiFromJob_User();
   UpdateUiFromJob_Ssh();
}

void EditConsoleJobDialog::UpdateUiFromJob_Basic()
{
   auto consoleJob = static_cast<AbstractConsoleJob*>(job);
   ui->nameEdit->setText(job->GetName().c_str());
   ui->commandEdit->setText(consoleJob->GetCommand().c_str());
   ui->parametersEdit->setText(consoleJob->GetCommandParameters().c_str());
   ui->expectedCodeSpinBox->setValue(consoleJob->GetExpectedReturnCode());
}

void EditConsoleJobDialog::UpdateUiFromJob_User()
{
/*   auto castJob = dynamic_cast<UserConsoleJob*>(job);
   if (!castJob)
      return;
   ui->useParserBox->setChecked(castJob->GetMiniDescriptionParserCommand() != "");
   ui->parserEdit->setText(castJob->GetMiniDescriptionParserCommand().c_str());
   ui->parserUsesBufferBox->setChecked(castJob->IsParsingUsingBuffer());
   UpdateParserUi();

   ui->expectedOutputTextEdit->setPlainText(castJob->GetExpectedOutput().c_str());
   const bool isOutputUsedAsCondition = (castJob->GetExpectedOutput() != "");
   ui->successTypeBox->setCurrentIndex(isOutputUsedAsCondition ? 1 : 0);

   vector<string> userAttachments;
   castJob->GetUserAttachments(userAttachments);
   for (const auto& it : userAttachments)
      ui->attachmentsWidget->addItem(QString(it.c_str()));*/
}

void EditConsoleJobDialog::UpdateUiFromJob_Ssh()
{
   auto castJob = dynamic_cast<SshConsoleJob*>(job);
   if (!castJob)
      return;

   //ui->attachOutputBox->setChecked(castJob->a);
   ui->useParserBox->setChecked(castJob->GetMiniDescriptionParserCommand() != "");
   ui->parserEdit->setText(castJob->GetMiniDescriptionParserCommand().c_str());
   ui->parserUsesBufferBox->setChecked(castJob->IsParsingUsingBuffer());
   UpdateParserUi();

   ui->expectedOutputTextEdit->setPlainText(castJob->GetExpectedOutput().c_str());
   const bool isOutputUsedAsCondition = (castJob->GetExpectedOutput() != "");
   ui->successTypeBox->setCurrentIndex(isOutputUsedAsCondition ? 1 : 0);

   vector<string> userAttachments;
   castJob->GetUserAttachments(userAttachments);
   for (const auto& it : userAttachments)
      ui->attachmentsWidget->addItem(QString(it.c_str()));
}

void EditConsoleJobDialog::UpdateJobFromUi()
{
   auto consoleJob = static_cast<AbstractConsoleJob*>(job);
   job->SetName(ui->nameEdit->text().toStdString());
   consoleJob->SetCommand(ui->commandEdit->text().toStdString());
   consoleJob->SetCommandParameters(ui->parametersEdit->text().toStdString());
   consoleJob->SetExpectedReturnCode(ui->expectedCodeSpinBox->value());
}

void EditConsoleJobDialog::UpdateParserUi()
{
   const bool parserEnabled = ui->useParserBox->isChecked();
   ui->parserLabel->setEnabled(parserEnabled);
   ui->parserEdit->setEnabled(parserEnabled);
   ui->parserUsesBufferBox->setEnabled(parserEnabled);

}

void EditConsoleJobDialog::on_successTypeBox_currentIndexChanged(int index)
{
   ui->successTypeWidget->setCurrentIndex(index);
}

void EditConsoleJobDialog::on_successTypeBox_2_activated(const QString &arg1)
{
    ui->fileOutputEdit->setEnabled(arg1 == "File");
}

void EditConsoleJobDialog::on_useParserBox_clicked()
{
   UpdateParserUi();
}

void EditConsoleJobDialog::on_useCurrentClientBox_clicked()
{
    const bool customTargetEnabled = !ui->useCurrentClientBox->isChecked();
    ui->userLabel->setEnabled(customTargetEnabled);
    ui->userEdit->setEnabled(customTargetEnabled);
    ui->hostLabel->setEnabled(customTargetEnabled);
    ui->hostEdit->setEnabled(customTargetEnabled);
}

void EditConsoleJobDialog::on_addAttachmentButton_clicked()
{
   // TODO : create popup box to enter new attachment.
   // Put a file browsing button too.
   ui->attachmentsWidget->addItem("NewAttachment.txt");
}

void EditConsoleJobDialog::on_removeAttachmentButton_clicked()
{
   QListWidgetItem* item = ui->attachmentsWidget->takeItem(ui->attachmentsWidget->currentRow());
   delete item;
}
