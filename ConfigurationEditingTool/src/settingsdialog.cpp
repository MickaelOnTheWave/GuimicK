#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QFileDialog>

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
   InitializeCssSelectionWidget();
   InitializeReportFolderSelectionWidget();
   UpdateUiFromConfiguration();

   SetCssControlsVisible(true);
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
   if (agent->GetName() == "")
      agent->SetName("Task Manager Agent");
}

void SettingsDialog::InitializeCssSelectionWidget()
{
   const QString fileFilter = "Css file (*.css)";
   const QString defaultFile = "";
   ui->cssWidget->InitializeAsFile("Css File",
                                   "Select Css File",
                                   defaultFile,
                                   fileFilter);
}

void SettingsDialog::InitializeReportFolderSelectionWidget()
{
   const QString defaultFolder = "";
   ui->reportFolderWidget->InitializeAsFolder(
            "Report Folder",
            "Choose a folder where the report will be created",
            defaultFolder
            );
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
   agent->SetName(GetValue(ui->agentNameEdit));

   EmailData newEmailData;
   newEmailData.SetAddress(GetValue(ui->emailEdit));
   newEmailData.SetPassword(GetValue(ui->passwordEdit));
   newEmailData.SetSmtpServer(GetValue(ui->smtpServerEdit));
   newEmailData.SetSmtpPort(ui->smtpPortBox->value());
   newEmailData.SetUseSsl(ui->smtpSslCheckBox->isChecked());
   agent->SetEmailData(newEmailData);

   agent->SetReportFile("hardcodedFile.html");
   agent->SetReportFolder(ui->reportFolderWidget->GetPath().toStdString());
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
   configuration->SetReportCss(ui->cssWidget->GetPath().toStdString());
}

void SettingsDialog::UpdateReportDispatchingFromUi()
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
   SetValue(ui->agentNameEdit, agent->GetName());

   const EmailData emailData = agent->GetEmailData();
   SetValue(ui->emailEdit, emailData.GetAddress());
   SetValue(ui->passwordEdit, emailData.GetPassword());
   SetValue(ui->smtpServerEdit, emailData.GetSmtpServer());
   ui->smtpPortBox->setValue(emailData.GetSmtpPort());
   ui->smtpSslCheckBox->setChecked(emailData.GetUseSsl());

   ui->reportFolderWidget->SetPath(agent->GetReportFolder().c_str());
}

void SettingsDialog::UpdateUiFromClient()
{
   SetValue(ui->clientNameEdit, configuration->GetClient()->GetName());
}

void SettingsDialog::UpdateReportTypeFromConfiguration()
{
   const string reportType = configuration->GetReportType();
   const bool isReportTypeHtml = (reportType == "html");
   SetCssControlsVisible(isReportTypeHtml);
   ui->reportFormatBox->setCurrentIndex(isReportTypeHtml ? 1 : 0);

   ui->cssWidget->SetPath(configuration->GetReportCss().c_str());
}

void SettingsDialog::UpdateReportDispatchingFromConfiguration()
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

void SettingsDialog::SetCssControlsVisible(const bool value)
{
   ui->cssWidget->setVisible(value);
}

void SettingsDialog::SetReportDispatchControls(const int index)
{
   ui->reportDispatchBox->setCurrentIndex(index);
   ui->dispatcherWidget->setCurrentIndex(index);
}

void SettingsDialog::on_reportFormatBox_currentIndexChanged(const QString &arg1)
{
   SetCssControlsVisible(arg1 == "HTML");
}

void SettingsDialog::on_reportDispatchBox_currentIndexChanged(int index)
{
   ui->dispatcherWidget->setCurrentIndex(index);
}

