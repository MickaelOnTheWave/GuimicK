#include "settingsuiupdater.h"

#include "ui_settingsdialog.h"

using namespace std;

namespace
{
   void SetValue(QLineEdit* lineEdit, const wstring& value)
   {
      lineEdit->setText(QString::fromStdWString(value));
   }
}

void SettingsUiUpdater::Update(Ui::SettingsDialog* ui, StandaloneConfiguration* configuration)
{
   SettingsUiUpdater updater(ui, configuration);
   updater.UpdateAll();
}

SettingsUiUpdater::SettingsUiUpdater(Ui::SettingsDialog* _ui,
                                     StandaloneConfiguration* _configuration)
   : SettingsUpdater(_ui, _configuration)
{
}

void SettingsUiUpdater::UpdateAgentData()
{
   Agent* agent = configuration->GetAgent();
   SetValue(ui->agentNameEdit, agent->GetName());
   UpdateEmailData(agent->GetEmailData());
   ui->reportFolderWidget->SetPath(QString::fromStdWString(agent->GetReportFolder()));
}

void SettingsUiUpdater::UpdateClientData()
{
   SetValue(ui->clientNameEdit, configuration->GetClient()->GetName());

   const std::wstring sevenZipPath =
         configuration->GetClient()->GetProperty(L"7zipExecutable");
   ui->sevenZipWidget->SetPath(QString::fromStdWString(sevenZipPath));
}

void SettingsUiUpdater::UpdateEmailData(const EmailData& emailData)
{
   SetValue(ui->emailEdit, emailData.GetAddress());
   SetValue(ui->passwordEdit, emailData.GetPassword());
   SetValue(ui->smtpServerEdit, emailData.GetSmtpServer());
   ui->smtpPortBox->setValue(emailData.GetSmtpPort());
   ui->smtpSslCheckBox->setChecked(emailData.GetUseSsl());
}

void SettingsUiUpdater::UpdateReportType()
{
   const wstring reportType = configuration->GetReportType();
   const bool isReportTypeHtml = (reportType == L"html");
   ui->cssWidget->setVisible(isReportTypeHtml);
   ui->reportFormatBox->setCurrentIndex(isReportTypeHtml ? 1 : 0);

   ui->cssWidget->SetPath(QString::fromStdWString(configuration->GetReportCss()));
}

void SettingsUiUpdater::UpdateReportDispatching()
{
   const wstring reportDispatching = configuration->GetReportDispatching();
   if (reportDispatching == L"email")
      SetReportDispatchControls(1);
   else if (reportDispatching == L"console")
      SetReportDispatchControls(2);
   else if (reportDispatching == L"file")
      SetReportDispatchControls(0);
   else
      SetReportDispatchControls(0);
}

void SettingsUiUpdater::UpdateOtherData()
{
   ui->shutdownCheckBox->setChecked(configuration->GetLocalShutdown());
   SetValue(ui->masterEmailEdit, configuration->GetMasterEmail());
}

void SettingsUiUpdater::SetReportDispatchControls(const int index)
{
   ui->reportDispatchBox->setCurrentIndex(index);
   ui->dispatcherWidget->setCurrentIndex(index);
}
