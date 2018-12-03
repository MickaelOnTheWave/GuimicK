#include "settingsconfigurationupdater.h"

#include "ui_settingsdialog.h"

using namespace std;

namespace
{
   string GetValue(QLineEdit* lineEdit)
   {
      return lineEdit->text().toStdString();
   }
}

void SettingsConfigurationUpdater::Update(Ui::SettingsDialog* ui, StandaloneConfiguration* configuration)
{
   SettingsConfigurationUpdater updater(ui, configuration);
   updater.UpdateAll();
}

SettingsConfigurationUpdater::SettingsConfigurationUpdater(Ui::SettingsDialog* _ui,
                                     StandaloneConfiguration* _configuration)
   : SettingsUpdater(_ui, _configuration)
{
}

void SettingsConfigurationUpdater::UpdateAgentData()
{
   Agent* agent = configuration->GetAgent();
   agent->SetName(GetValue(ui->agentNameEdit));
   agent->SetEmailData(CreateEmailData());
   agent->SetReportFolder(ui->reportFolderWidget->GetPath().toStdString());
}

void SettingsConfigurationUpdater::UpdateClientData()
{
   configuration->GetClient()->SetName(GetValue(ui->clientNameEdit));
}

void SettingsConfigurationUpdater::UpdateReportType()
{
   const QString uiReportType = ui->reportFormatBox->currentText();
   string confReportType;
   if (uiReportType == "HTML")
      confReportType = "html";
   else// if (uiReportType == "Plain Text")
      confReportType = "text";

   configuration->SetReportType(confReportType);
   configuration->SetReportCss(ui->cssWidget->GetPath().toStdString());
}

void SettingsConfigurationUpdater::UpdateReportDispatching()
{
   const QString uiReportDispatching = ui->reportDispatchBox->currentText();
   string confReportDispatching;
   if (uiReportDispatching == "Email")
      confReportDispatching = "email";
   else if (uiReportDispatching == "Console")
      confReportDispatching = "console";
   else // if (uiReportDispatching == "Local files")
      confReportDispatching = "file";

   configuration->SetReportDispatching(confReportDispatching);
}

void SettingsConfigurationUpdater::UpdateOtherData()
{
   configuration->SetLocalShutdown(ui->shutdownCheckBox->isChecked());
   configuration->SetMasterEmail(GetValue(ui->masterEmailEdit));
}

EmailData SettingsConfigurationUpdater::CreateEmailData()
{
   EmailData emailData;
   emailData.SetAddress(GetValue(ui->emailEdit));
   emailData.SetPassword(GetValue(ui->passwordEdit));
   emailData.SetSmtpServer(GetValue(ui->smtpServerEdit));
   emailData.SetSmtpPort(ui->smtpPortBox->value());
   emailData.SetUseSsl(ui->smtpSslCheckBox->isChecked());
   return emailData;
}
