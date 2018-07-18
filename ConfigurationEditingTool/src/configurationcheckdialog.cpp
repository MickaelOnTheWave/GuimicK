#include "configurationcheckdialog.h"
#include "ui_configurationcheckdialog.h"

using namespace std;

ConfigurationCheckDialog::ConfigurationCheckDialog(QWidget *parent) :
   QDialog(parent),
   ui(new Ui::ConfigurationCheckDialog)
{
   ui->setupUi(this);
}

ConfigurationCheckDialog::~ConfigurationCheckDialog()
{
   delete ui;
}

void ConfigurationCheckDialog::Show(const bool isFatalError, const std::vector<std::string>& errors)
{
   ConfigurationCheckDialog dialog;
   dialog.SetupControls(isFatalError, errors);
   dialog.exec();
}

void ConfigurationCheckDialog::SetupControls(const bool isFatalError, const std::vector<std::string>& errors)
{
   if (isFatalError)
      ui->descriptionLabel->setText("Fatal errors found in configuration. It is not usable in this state.");
   else if (errors.size() > 0)
      ui->descriptionLabel->setText("Minor errors found in configuration. See errors list for details.");
   else
      ui->descriptionLabel->setText("Configuration successfully loaded");

   ui->descriptionLabel->updateGeometry();
   ui->descriptionLabel->update();

   if (errors.size() > 0)
   {
      QStringList qtErrors;
      for (auto& it : errors)
         qtErrors.push_back(QString(it.c_str()));
      ui->listWidget->addItems(qtErrors);
   }
   else
      ui->detailsFrame->setVisible(false);
}
