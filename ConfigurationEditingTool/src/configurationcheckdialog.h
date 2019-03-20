#ifndef CONFIGURATIONCHECKDIALOG_H
#define CONFIGURATIONCHECKDIALOG_H

#include <QDialog>

namespace Ui {
   class ConfigurationCheckDialog;
}

class ConfigurationCheckDialog : public QDialog
{
   Q_OBJECT

public:
   explicit ConfigurationCheckDialog(QWidget *parent = 0);
   ~ConfigurationCheckDialog();

   static void Show(const bool isFatalError,
                    const std::vector<std::wstring>& errors);

private:
   void SetupControls(const bool isFatalError,
                      const std::vector<std::wstring>& errors);

   Ui::ConfigurationCheckDialog *ui;
};

#endif // CONFIGURATIONCHECKDIALOG_H
