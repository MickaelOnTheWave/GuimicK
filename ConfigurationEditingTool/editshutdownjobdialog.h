#ifndef EDITSHUTDOWNJOBDIALOG_H
#define EDITSHUTDOWNJOBDIALOG_H

#include "abstracteditjobdialog.h"

namespace Ui {
   class EditShutdownJobDialog;
}

class EditShutdownJobDialog : public AbstractEditJobDialog
{
   Q_OBJECT

public:
   explicit EditShutdownJobDialog(AbstractJob* _job);
   virtual ~EditShutdownJobDialog();

private slots:
   void on_okButton_clicked();

   void on_cancelButton_clicked();

private:
   void UpdateUiFromJob();
   void UpdateJobFromUi();

   Ui::EditShutdownJobDialog *ui;
};

#endif // EDITSHUTDOWNJOBDIALOG_H
