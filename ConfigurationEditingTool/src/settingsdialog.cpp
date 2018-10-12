#include "settingsdialog.h"
#include "ui_settingsdialog.h"

using namespace std;

namespace
{
   string GetValue(QLineEdit* lineEdit)
   {
      return lineEdit->text().toStdString();
   }

   void SetValue(QLineEdit* lineEdit, const string& value)
   {
      lineEdit->setText(value.c_str());
   }
}

SettingsDialog::SettingsDialog(Client* _client, SelfIdentity* _agent,
                               QWidget *parent) :
   QDialog(parent),
   client(_client), agent(_agent),
   ui(new Ui::SettingsDialog)
{
   ui->setupUi(this);
   SetDefaultValues();
   UpdateUiFromAgent();
   UpdateUiFromClient();
}

SettingsDialog::~SettingsDialog()
{
   delete ui;
}

void SettingsDialog::on_exitButtonBox_accepted()
{
   UpdateAgentFromUi();
   UpdateClientFromUi();
   accept();
}

void SettingsDialog::on_exitButtonBox_rejected()
{
   reject();
}

void SettingsDialog::SetDefaultValues()
{
   if (client->GetName() == "")
      client->SetName("Local Client");
   if (agent->name == "")
      agent->name = "Task Manager Agent";
}

void SettingsDialog::UpdateAgentFromUi()
{
   agent->name = GetValue(ui->agentNameEdit);
   agent->email = GetValue(ui->emailEdit);
   agent->emailPassword = GetValue(ui->passwordEdit);
   agent->emailSmtpServer = GetValue(ui->smtpServerEdit);
   agent->emailSmtpPort = ui->smtpPortBox->value();
   agent->emailUseSsl = ui->smtpSslCheckBox->isChecked();
}

void SettingsDialog::UpdateClientFromUi()
{
   client->SetName(GetValue(ui->clientNameEdit));
}

void SettingsDialog::UpdateUiFromAgent()
{
   SetValue(ui->agentNameEdit, agent->name);
   SetValue(ui->emailEdit, agent->email);
   SetValue(ui->passwordEdit, agent->emailPassword);
   SetValue(ui->smtpServerEdit, agent->emailSmtpServer);
   ui->smtpPortBox->setValue(agent->emailSmtpPort);
   ui->smtpSslCheckBox->setChecked(agent->emailUseSsl);
}

void SettingsDialog::UpdateUiFromClient()
{
   SetValue(ui->clientNameEdit, client->GetName());
}
