#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(const std::string versionInfo,
                         const ConfigurationType configurationType,
                         QWidget *parent) :
   QDialog(parent),
   ui(new Ui::AboutDialog)
{
   ui->setupUi(this);
   SetExeInfo(versionInfo, configurationType);
}

AboutDialog::~AboutDialog()
{
   delete ui;
}

void AboutDialog::SetExeInfo(const std::string versionInfo,
                             const ConfigurationType configurationType)
{
   QString fullExeInfo("Configuration Editor v");
   fullExeInfo.append(versionInfo.c_str()).append("\t (");
   fullExeInfo.append(CreateModeString(configurationType)).append(")");
   ui->labelExeInfo->setText(fullExeInfo);
}

QString AboutDialog::CreateModeString(const ConfigurationType configurationType) const
{
   switch (configurationType)
   {
      case ServerConfigurationType : return "Server";
      case ClientConfigurationType : return "Client";
      case StandaloneConfigurationType : return "Standalone";
      default : return "Unsupported";
   }
}
