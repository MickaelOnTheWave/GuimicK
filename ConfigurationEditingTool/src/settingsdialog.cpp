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

SettingsDialog::SettingsDialog(StandaloneConfiguration* _configuration,
                               QWidget *parent) :
   QDialog(parent),
   configuration(_configuration),
   ui(new Ui::SettingsDialog)
{
   ui->setupUi(this);
   SetDefaultValues();
   UpdateUiFromConfiguration();
}

SettingsDialog::~SettingsDialog()
{
   delete ui;
}

void SettingsDialog::on_exitButtonBox_accepted()
{
   UpdateConfigurationFromUi();
   accept();
}

void SettingsDialog::on_exitButtonBox_rejected()
{
   reject();
}

void SettingsDialog::SetDefaultValues()
{
   Client* client = configuration->GetClient();
   Agent* agent = configuration->GetAgent();
   if (client->GetName() == "")
      client->SetName("Local Client");
   if (agent->name == "")
      agent->name = "Task Manager Agent";
}

void SettingsDialog::UpdateConfigurationFromUi()
{
   UpdateAgentFromUi();
   UpdateClientFromUi();

   UpdateReportTypeFromUi();
   UpdateReportDispatchingFromUi();
   configuration->SetLocalShutdown(ui->shutdownCheckBox->isChecked());
   configuration->SetMasterEmail(GetValue(ui->masterEmailEdit));
}

void SettingsDialog::UpdateAgentFromUi()
{
   Agent* agent = configuration->GetAgent();
   agent->name = GetValue(ui->agentNameEdit);
   agent->email = GetValue(ui->emailEdit);
   agent->emailPassword = GetValue(ui->passwordEdit);
   agent->emailSmtpServer = GetValue(ui->smtpServerEdit);
   agent->emailSmtpPort = ui->smtpPortBox->value();
   agent->emailUseSsl = ui->smtpSslCheckBox->isChecked();
}

void SettingsDialog::UpdateClientFromUi()
{
   configuration->GetClient()->SetName(GetValue(ui->clientNameEdit));
}

void SettingsDialog::UpdateReportTypeFromUi()
{
   const QString uiReportType = ui->reportFormatBox->currentText();
   string confReportType;
   if (uiReportType == "HTML")
      confReportType = "html";
   else// if (uiReportType == "Plain Text")
      confReportType = "text";

   configuration->SetReportType(confReportType);
}

void SettingsDialog::UpdateReportDispatchingFromUi()
{
   const QString uiReportDispatching = ui->reportDispatchBox->currentText();
   string confReportDispatching;
   if (uiReportDispatching == "Email")
      confReportDispatching = "email";
   else// if (uiReportDispatching == "Console")
      confReportDispatching = "console";

   configuration->SetReportDispatching(confReportDispatching);
}

void SettingsDialog::UpdateUiFromConfiguration()
{
   UpdateUiFromAgent();
   UpdateUiFromClient();

   UpdateReportTypeFromConfiguration();
   UpdateReportDispatchingFromConfiguration();
   ui->shutdownCheckBox->setChecked(configuration->GetLocalShutdown());
   SetValue(ui->masterEmailEdit, configuration->GetMasterEmail());
}

void SettingsDialog::UpdateUiFromAgent()
{
   Agent* agent = configuration->GetAgent();
   SetValue(ui->agentNameEdit, agent->name);
   SetValue(ui->emailEdit, agent->email);
   SetValue(ui->passwordEdit, agent->emailPassword);
   SetValue(ui->smtpServerEdit, agent->emailSmtpServer);
   ui->smtpPortBox->setValue(agent->emailSmtpPort);
   ui->smtpSslCheckBox->setChecked(agent->emailUseSsl);
}

void SettingsDialog::UpdateUiFromClient()
{
   SetValue(ui->clientNameEdit, configuration->GetClient()->GetName());
}

void SettingsDialog::UpdateReportTypeFromConfiguration()
{
   const string reportType = configuration->GetReportType();
   if (reportType == "html")
      ui->reportFormatBox->setCurrentIndex(1);
   else // if (reportType == "text")
      ui->reportFormatBox->setCurrentIndex(0);
}

void SettingsDialog::UpdateReportDispatchingFromConfiguration()
{
   const string reportDispatching = configuration->GetReportDispatching();
   if (reportDispatching == "email")
      ui->reportDispatchBox->setCurrentIndex(1);
   else if (reportDispatching == "console")
      ui->reportDispatchBox->setCurrentIndex(0);
   else
      ui->reportDispatchBox->setCurrentIndex(0);
}
