#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include "configurationtype.h"

namespace Ui {
   class AboutDialog;
}

class AboutDialog : public QDialog
{
   Q_OBJECT

public:
   explicit AboutDialog(const ConfigurationType configurationType,
                        QWidget *parent = 0);
   ~AboutDialog();

private:
   void SetExeInfo(const ConfigurationType configurationType);

   QString CreateModeString(const ConfigurationType configurationType) const;

   Ui::AboutDialog *ui;
};

#endif // ABOUTDIALOG_H
