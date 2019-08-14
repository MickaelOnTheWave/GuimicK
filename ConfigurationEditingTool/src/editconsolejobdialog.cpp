#include "editconsolejobdialog.h"
#include "ui_editconsolejobdialog.h"

#include "choosefiledialog.h"
#include "editorversion.h"
#include "sshconsolejob.h"
#include "userconsolejob.h"

using namespace std;

EditConsoleJobDialog::EditConsoleJobDialog(AbstractJob* _job) :
   AbstractEditJobDialog(_job),
   ui(new Ui::EditConsoleJobDialog)
{
   ui->setupUi(this);
   InitializeUi();
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
   UpdateJobTypeLabel();
   UpdateUiFromJob_Basic();
   UpdateUiFromJob_User();
}

void EditConsoleJobDialog::UpdateJobFromUi()
{
   UpdateJobFromUi_Basic();
   UpdateJobFromUi_User();
}

void EditConsoleJobDialog::InitializeUi()
{
   ui->tabWidget->removeTab(2); // Ssh jobs must be this client.
   if (EditorVersion::HasDevelopmentFeatures() == false)
      ui->tabWidget->removeTab(1); // Remove User Properties tab
}

void EditConsoleJobDialog::UpdateJobTypeLabel()
{
   if (dynamic_cast<UserConsoleJob*>(job))
      ui->jobTypeLabel->setText("Server Command");
   else if (dynamic_cast<SshConsoleJob*>(job))
      ui->jobTypeLabel->setText("Client Command");
   else
      ui->jobTypeLabel->setText("Internal Command");
}

void EditConsoleJobDialog::UpdateUiFromJob_Basic()
{
   auto consoleJob = static_cast<AbstractConsoleJob*>(job);
   ui->nameEdit->setText(QString::fromStdWString(job->GetName()));
   ui->commandEdit->setText(QString::fromStdWString(consoleJob->GetCommand()));
   ui->parametersEdit->setText(QString::fromStdWString(consoleJob->GetCommandParameters()));
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
   const wstring miniDescription = userJob->GetMiniDescriptionParserCommand();
   ui->useParserBox->setChecked(miniDescription != L"");
   ui->parserEdit->setText(QString::fromStdWString(miniDescription));
   ui->parserUsesBufferBox->setChecked(userJob->IsParsingUsingBuffer());
   UpdateParserUi();

   ui->expectedOutputTextEdit->setPlainText(QString::fromStdWString(userJob->GetExpectedOutput()));
   const bool isOutputUsedAsCondition = (userJob->GetExpectedOutput() != L"");
   ui->successTypeBox->setCurrentIndex(isOutputUsedAsCondition ? 1 : 0);

   ui->fileOutputEdit->setText(QString::fromStdWString(userJob->GetOutputFile()));
   const bool isOutputToFile = (userJob->GetOutputFile() != L"");
   ui->outputTypeBox->setCurrentIndex(isOutputToFile ? 1 : 0);
   ui->fileOutputEdit->setEnabled(isOutputToFile);

   vector<wstring> userAttachments;
   userJob->GetUserAttachments(userAttachments);
   for (const auto& it : userAttachments)
      ui->attachmentsWidget->addItem(QString::fromStdWString(it.c_str()));
}

void EditConsoleJobDialog::UpdateJobFromUi_Basic()
{
   auto consoleJob = static_cast<AbstractConsoleJob*>(job);
   job->SetName(ui->nameEdit->text().toStdWString());
   consoleJob->SetCommand(ui->commandEdit->text().toStdWString());
   consoleJob->SetCommandParameters(ui->parametersEdit->text().toStdWString());
   consoleJob->SetExpectedReturnCode(ui->expectedCodeSpinBox->value());
}

void EditConsoleJobDialog::UpdateJobFromUi_User()
{
   UserConsoleJob* userJob = GetUserConsoleJob();

   const QString parserCommand = (ui->useParserBox->isChecked()) ? ui->parserEdit->text() : QString("");
   userJob->SetMiniDescriptionParserCommand(parserCommand.toStdWString());
   userJob->SetParsingUsingBuffer(ui->parserUsesBufferBox->isChecked());

   if (ui->successTypeBox->currentIndex() == 1)
      userJob->SetExpectedOutput(ui->expectedOutputTextEdit->toPlainText().toStdWString());
   else if (ui->successTypeBox->currentIndex() == 0)
      userJob->SetExpectedReturnCode(ui->expectedCodeSpinBox->value());

   if (ui->outputTypeBox->currentIndex() == 0)
      userJob->SetOutputToBuffer();
   else
      userJob->SetOutputTofile(ui->fileOutputEdit->text().toStdWString());

   userJob->SetAttachOutput(ui->attachOutputBox->isChecked());

   userJob->EmptyUserAttachments();
   for (int i=0; i<ui->attachmentsWidget->count(); ++i)
      userJob->AddUserAttachment(ui->attachmentsWidget->item(i)->text().toStdWString());
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
