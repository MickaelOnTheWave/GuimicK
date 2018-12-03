#include "settingsuiupdater.h"

#include "ui_settingsdialog.h"

using namespace std;

namespace
{
   void SetValue(QLineEdit* lineEdit, const string& value)
   {
      lineEdit->setText(value.c_str());
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
   ui->reportFolderWidget->SetPath(agent->GetReportFolder().c_str());
}

void SettingsUiUpdater::UpdateClientData()
{
   SetValue(ui->clientNameEdit, configuration->GetClient()->GetName());
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
   const string reportType = configuration->GetReportType();
   const bool isReportTypeHtml = (reportType == "html");
   ui->cssWidget->setVisible(isReportTypeHtml);
   ui->reportFormatBox->setCurrentIndex(isReportTypeHtml ? 1 : 0);

   ui->cssWidget->SetPath(configuration->GetReportCss().c_str());
}

void SettingsUiUpdater::UpdateReportDispatching()
{
   const string reportDispatching = configuration->GetReportDispatching();
   if (reportDispatching == "email")
      SetReportDispatchControls(1);
   else if (reportDispatching == "console")
      SetReportDispatchControls(0);
   else if (reportDispatching == "file")
      SetReportDispatchControls(2);
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
