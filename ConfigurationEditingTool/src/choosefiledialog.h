#ifndef CHOOSEFILEDIALOG_H
#define CHOOSEFILEDIALOG_H

#include <QDialog>

namespace Ui {
   class ChooseFileDialog;
}

class ChooseFileDialog : public QDialog
{
   Q_OBJECT

public:
   explicit ChooseFileDialog(QWidget *parent = 0);
   ~ChooseFileDialog();

   QString GetFilename() const;

private slots:
   void on_buttonBox_accepted();

   void on_buttonBox_rejected();

   void on_chooseFileButton_clicked();

private:
   Ui::ChooseFileDialog *ui;
};

#endif // CHOOSEFILEDIALOG_H
