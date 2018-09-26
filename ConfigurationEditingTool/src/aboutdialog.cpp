#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(const std::string versionInfo, const bool isServerMode,
                         QWidget *parent) :
   QDialog(parent),
   ui(new Ui::AboutDialog)
{
   ui->setupUi(this);
   SetExeInfo(versionInfo, isServerMode);
}

AboutDialog::~AboutDialog()
{
   delete ui;
}

void AboutDialog::SetExeInfo(const std::string versionInfo, const bool isServerMode)
{
   QString fullExeInfo("Configuration Editor v");
   fullExeInfo.append(versionInfo.c_str()).append("\t (");
   fullExeInfo.append(isServerMode ? "Server" : "Client").append(")");
   ui->labelExeInfo->setText(fullExeInfo);
}
