#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
   class AboutDialog;
}

class AboutDialog : public QDialog
{
   Q_OBJECT

public:
   explicit AboutDialog(const std::string versionInfo, const bool isServerMode,
                        QWidget *parent = 0);
   ~AboutDialog();

private:
   void SetExeInfo(const std::string versionInfo, const bool isServerMode);

   Ui::AboutDialog *ui;
};

#endif // ABOUTDIALOG_H
