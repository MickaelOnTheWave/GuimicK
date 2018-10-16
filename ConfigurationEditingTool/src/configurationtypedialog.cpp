#include "configurationtypedialog.h"
#include "ui_configurationtypedialog.h"

ConfigurationTypeDialog::ConfigurationTypeDialog(QWidget *parent) :
   QDialog(parent),
   ui(new Ui::ConfigurationTypeDialog)
{
   ui->setupUi(this);
}

ConfigurationTypeDialog::~ConfigurationTypeDialog()
{
   delete ui;
}

ConfigurationType ConfigurationTypeDialog::GetChosenType() const
{
   if (ui->serverButton->isChecked())
      return ServerConfigurationType;
   else if (ui->clientButton->isChecked())
      return ClientConfigurationType;
   else //if (ui->standaloneButton->isChecked())
      return StandaloneConfigurationType;
}
