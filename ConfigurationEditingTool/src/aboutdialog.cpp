#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#include "editorversion.h"

AboutDialog::AboutDialog(const ConfigurationType configurationType,
                         QWidget *parent) :
   QDialog(parent),
   ui(new Ui::AboutDialog)
{
   ui->setupUi(this);
   SetExeInfo(configurationType);
}

AboutDialog::~AboutDialog()
{
   delete ui;
}

void AboutDialog::SetExeInfo(const ConfigurationType configurationType)
{
   const QString versionInfo = QString::fromStdWString(EditorVersion::GetVersionTag());
   QString fullExeInfo("Configuration Editor v");
   fullExeInfo.append(versionInfo).append("\t (");
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
