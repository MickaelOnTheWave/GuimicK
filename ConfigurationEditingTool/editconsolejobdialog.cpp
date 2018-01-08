#include "editconsolejobdialog.h"
#include "ui_editconsolejobdialog.h"

#include "choosefiledialog.h"
#include "sshconsolejob.h"
#include "userconsolejob.h"

using namespace std;

EditConsoleJobDialog::EditConsoleJobDialog(AbstractJob* _job) :
   AbstractEditJobDialog(_job),
   ui(new Ui::EditConsoleJobDialog)
{
   ui->setupUi(this);
   ui->tabWidget->removeTab(2); // Ssh jobs must be this client.
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
}

void EditConsoleJobDialog::UpdateJobFromUi()
{
   UpdateJobFromUi_Basic();
   UpdateJobFromUi_User();
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
   UserConsoleJob* userJob = GetUserConsoleJob();
   if (userJob)
      UpdateUiFromUserConsoleJob(userJob);
   else
      ui->tabWidget->removeTab(1);
}

void EditConsoleJobDialog::UpdateUiFromUserConsoleJob(UserConsoleJob* userJob)
{
   ui->useParserBox->setChecked(userJob->GetMiniDescriptionParserCommand() != "");
   ui->parserEdit->setText(userJob->GetMiniDescriptionParserCommand().c_str());
   ui->parserUsesBufferBox->setChecked(userJob->IsParsingUsingBuffer());
   UpdateParserUi();

   ui->expectedOutputTextEdit->setPlainText(userJob->GetExpectedOutput().c_str());
   const bool isOutputUsedAsCondition = (userJob->GetExpectedOutput() != "");
   ui->successTypeBox->setCurrentIndex(isOutputUsedAsCondition ? 1 : 0);

   ui->fileOutputEdit->setText(userJob->GetOutputFile().c_str());
   const bool isOutputToFile = (userJob->GetOutputFile() != "");
   ui->outputTypeBox->setCurrentIndex(isOutputToFile ? 1 : 0);
   ui->fileOutputEdit->setEnabled(isOutputToFile);

   vector<string> userAttachments;
   userJob->GetUserAttachments(userAttachments);
   for (const auto& it : userAttachments)
      ui->attachmentsWidget->addItem(QString(it.c_str()));
}

void EditConsoleJobDialog::UpdateJobFromUi_Basic()
{
   auto consoleJob = static_cast<AbstractConsoleJob*>(job);
   job->SetName(ui->nameEdit->text().toStdString());
   consoleJob->SetCommand(ui->commandEdit->text().toStdString());
   consoleJob->SetCommandParameters(ui->parametersEdit->text().toStdString());
   consoleJob->SetExpectedReturnCode(ui->expectedCodeSpinBox->value());
}

void EditConsoleJobDialog::UpdateJobFromUi_User()
{
   UserConsoleJob* userJob = GetUserConsoleJob();

   const QString parserCommand = (ui->useParserBox->isChecked()) ? ui->parserEdit->text() : QString("");
   userJob->SetMiniDescriptionParserCommand(parserCommand.toStdString());
   userJob->SetParsingUsingBuffer(ui->parserUsesBufferBox->isChecked());

   if (ui->successTypeBox->currentIndex() == 1)
      userJob->SetExpectedOutput(ui->expectedOutputTextEdit->toPlainText().toStdString());
   else if (ui->successTypeBox->currentIndex() == 0)
      userJob->SetExpectedReturnCode(ui->expectedCodeSpinBox->value());

   if (ui->outputTypeBox->currentIndex() == 0)
      userJob->SetOutputToBuffer();
   else
      userJob->SetOutputTofile(ui->fileOutputEdit->text().toStdString());

   userJob->EmptyUserAttachments();
   for (int i=0; i<ui->attachmentsWidget->count(); ++i)
      userJob->AddUserAttachment(ui->attachmentsWidget->item(i)->text().toStdString());
}

UserConsoleJob* EditConsoleJobDialog::GetUserConsoleJob()
{
   auto userJob = dynamic_cast<UserConsoleJob*>(job);
   if (userJob)
      return userJob;
   else if (auto sshJob = dynamic_cast<SshConsoleJob*>(job))
   {
      auto childJob = sshJob->GetRemoteJob();
      if ((userJob = dynamic_cast<UserConsoleJob*>(childJob)) != nullptr)
         return userJob;
   }

   return nullptr;
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

void EditConsoleJobDialog::on_outputTypeBox_activated(const QString &arg1)
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
   ChooseFileDialog dialog;
   if (dialog.exec() == QDialog::Accepted)
      ui->attachmentsWidget->addItem(dialog.GetFilename());
}

void EditConsoleJobDialog::on_removeAttachmentButton_clicked()
{
   QListWidgetItem* item = ui->attachmentsWidget->takeItem(ui->attachmentsWidget->currentRow());
   delete item;
}
