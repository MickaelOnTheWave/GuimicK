#ifndef EDITWAKEJOBDIALOG_H
#define EDITWAKEJOBDIALOG_H

#include "abstracteditjobdialog.h"

namespace Ui {
   class EditWakeJobDialog;
}

class EditWakeJobDialog : public AbstractEditJobDialog
{
   Q_OBJECT

public:
   explicit EditWakeJobDialog(AbstractJob* _job);
   virtual ~EditWakeJobDialog();

private slots:
   void on_okButton_clicked();

   void on_cancelButton_clicked();

private:
   void UpdateUiFromJob();
   void UpdateJobFromUi();

   Ui::EditWakeJobDialog *ui;
};

#endif // EDITWAKEJOBDIALOG_H
