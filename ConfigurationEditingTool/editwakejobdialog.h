#ifndef EDITWAKEJOBDIALOG_H
#define EDITWAKEJOBDIALOG_H

#include <QDialog>

namespace Ui {
   class EditWakeJobDialog;
}

class EditWakeJobDialog : public QDialog
{
   Q_OBJECT

public:
   explicit EditWakeJobDialog(QWidget *parent = 0);
   ~EditWakeJobDialog();

private:
   Ui::EditWakeJobDialog *ui;
};

#endif // EDITWAKEJOBDIALOG_H
