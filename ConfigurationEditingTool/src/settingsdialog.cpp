#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QFileDialog>

#include "editorversion.h"
#include "ostools.h"
#include "serverconfiguration.h"
#include "settingsconfigurationupdater.h"
#include "settingsuiupdater.h"

using namespace std;

bool IsConfigurationServer(StandaloneConfiguration* configuration)
{
   return (dynamic_cast<ServerConfiguration*>(configuration) != nullptr);
}

SettingsDialog::SettingsDialog(StandaloneConfiguration* _configuration,
                               QWidget *parent) :
   QDialog(parent),
   ui(new Ui::SettingsDialog),
   configuration(_configuration)
{
   ui->setupUi(this);
   SetDefaultValues();
   InitializeCssSelectionWidget();
   InitializeReportFolderSelectionWidget();
   Initialize7zipWidget();
   InitializeReportDispatchBox();

   ui->cssWidget->setVisible(true); // default : HTML
   ui->dispatcherWidget->setCurrentIndex(0); // default : Local files

   SettingsUiUpdater::Update(ui, configuration);

   if (IsConfigurationServer(configuration) == false)
      HideClientTab();

   if (EditorVersion::HasDevelopmentFeatures() == false)
      HideDevelopmentFeatures();
}

SettingsDialog::~SettingsDialog()
{
   delete ui;
}

void SettingsDialog::on_exitButtonBox_accepted()
{
   SettingsConfigurationUpdater::Update(ui, configuration);
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
   if (client->GetName() == L"")
      client->SetName(L"Local Client");
   if (agent->GetName() == L"")
      agent->SetName(L"Task Manager Agent");
}

void SettingsDialog::InitializeCssSelectionWidget()
{
   const QString fileFilter = "Css file (*.css)";
   const QString defaultFile = "";
   ui->cssWidget->InitializeAsExistingFile("Css File",
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

void SettingsDialog::Initialize7zipWidget()
{
   const QString fileFilter = "Executable file (*.exe)";
   const QString defaultFile = "";
   ui->sevenZipWidget->InitializeAsExistingFile(
        "7zip Executable",
        "Select 7zip Executable",
        defaultFile,
        fileFilter
   );
   ui->sevenZipWidget->setVisible(OsTools::IsOnWindows());
}

void SettingsDialog::InitializeReportDispatchBox()
{
   ui->reportDispatchBox->addItem("Local folder");
   if (EditorVersion::HasDevelopmentFeatures())
   {
      ui->reportDispatchBox->addItem("Email");
      ui->reportDispatchBox->addItem("Console");
   }
}

void SettingsDialog::HideClientTab()
{
   ui->settingsWidget->removeTab(1);
}

void SettingsDialog::HideDevelopmentFeatures()
{
   ui->agentNameLabel->setVisible(false);
   ui->agentNameEdit->setVisible(false);
   ui->reportDispatchLabel->setVisible(false);
   ui->reportDispatchBox->setVisible(false);
}

void SettingsDialog::on_reportFormatBox_currentIndexChanged(const QString &arg1)
{
   ui->cssWidget->setVisible(arg1 == "HTML");
}

void SettingsDialog::on_reportDispatchBox_currentIndexChanged(int index)
{
   ui->dispatcherWidget->setCurrentIndex(index);
}
