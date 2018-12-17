#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QFileDialog>
#include "settingsconfigurationupdater.h"
#include "settingsuiupdater.h"

using namespace std;

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

   SettingsUiUpdater::Update(ui, configuration);

   ui->cssWidget->setVisible(true);
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
   if (client->GetName() == "")
      client->SetName("Local Client");
   if (agent->GetName() == "")
      agent->SetName("Task Manager Agent");
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

void SettingsDialog::on_reportFormatBox_currentIndexChanged(const QString &arg1)
{
   ui->cssWidget->setVisible(arg1 == "HTML");
}

void SettingsDialog::on_reportDispatchBox_currentIndexChanged(int index)
{
   ui->dispatcherWidget->setCurrentIndex(index);
}
