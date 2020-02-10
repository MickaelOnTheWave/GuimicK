#include "settingsconfigurationupdater.h"

#include "ui_settingsdialog.h"

using namespace std;

namespace
{
   wstring GetValue(QLineEdit* lineEdit)
   {
      return lineEdit->text().toStdWString();
   }

   wstring FixPath(const wstring& value)
   {
      const wchar_t pathSeparator = L'/';
      wstring fixedValue = value;
      if (value.back() != pathSeparator)
         fixedValue += pathSeparator;
      return fixedValue;
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
   agent->SetReportFolder(FixPath(ui->reportFolderWidget->GetPath().toStdWString()));
   agent->SetReportFile(GetDefaultReportFilename());
}

void SettingsConfigurationUpdater::UpdateClientData()
{
   configuration->GetClient()->SetName(GetValue(ui->clientNameEdit));
   Update7zipPathProperty(ui->sevenZipWidget->GetPath());
}

void SettingsConfigurationUpdater::UpdateReportType()
{
   const QString uiReportType = ui->reportFormatBox->currentText();
   wstring confReportType;
   if (uiReportType == "HTML")
      confReportType = L"html";
   else// if (uiReportType == "Plain Text")
      confReportType = L"text";

   configuration->SetReportType(confReportType);
   configuration->SetReportCss(ui->cssWidget->GetPath().toStdWString());
}

void SettingsConfigurationUpdater::UpdateReportDispatching()
{
   const QString uiReportDispatching = ui->reportDispatchBox->currentText();
   wstring confReportDispatching;
   if (uiReportDispatching == "Email")
      confReportDispatching = L"email";
   else if (uiReportDispatching == "Console")
      confReportDispatching = L"console";
   else // if (uiReportDispatching == "Local files")
      confReportDispatching = L"file";

   configuration->SetReportDispatching(confReportDispatching);
}

void SettingsConfigurationUpdater::UpdateOtherData()
{
   configuration->SetLocalShutdown(ui->shutdownCheckBox->isChecked());
   configuration->SetMasterEmail(GetValue(ui->masterEmailEdit));
}

void SettingsConfigurationUpdater::Update7zipPathProperty(const QString& value)
{
   if (value != "")
      configuration->GetClient()->AddProperty(L"7zipExecutable", value.toStdWString());
}

EmailAccountData SettingsConfigurationUpdater::CreateEmailData()
{
   EmailAccountData emailData;
   emailData.SetAddress(GetValue(ui->emailEdit));
   emailData.SetPassword(GetValue(ui->passwordEdit));
   emailData.SetSmtpServer(GetValue(ui->smtpServerEdit));
   emailData.SetSmtpPort(ui->smtpPortBox->value());
   emailData.SetUseSsl(ui->smtpSslCheckBox->isChecked());
   return emailData;
}

std::wstring SettingsConfigurationUpdater::GetDefaultReportFilename() const
{
   if (configuration->GetReportType() == L"html")
      return L"report.html";
   else
      return L"report.txt";
}
