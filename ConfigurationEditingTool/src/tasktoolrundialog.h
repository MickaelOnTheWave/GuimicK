#ifndef TASKTOOLRUNDIALOG_H
#define TASKTOOLRUNDIALOG_H

#include <QDialog>

namespace Ui {
   class TaskToolRunDialog;
}

class TaskToolRunDialog : public QDialog
{
   Q_OBJECT

public:
   explicit TaskToolRunDialog(QWidget *parent = 0);
   ~TaskToolRunDialog();

private:
   Ui::TaskToolRunDialog *ui;
};

#endif // TASKTOOLRUNDIALOG_H
