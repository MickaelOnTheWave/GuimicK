#ifndef CONFIGURATIONTYPEDIALOG_H
#define CONFIGURATIONTYPEDIALOG_H

#include <QDialog>
#include "configurationtype.h"

namespace Ui {
   class ConfigurationTypeDialog;
}

class ConfigurationTypeDialog : public QDialog
{
   Q_OBJECT

public:
   explicit ConfigurationTypeDialog(QWidget *parent = 0);
   ~ConfigurationTypeDialog();

   ConfigurationType GetChosenType() const;

private:
   Ui::ConfigurationTypeDialog *ui;
};

#endif // CONFIGURATIONTYPEDIALOG_H
