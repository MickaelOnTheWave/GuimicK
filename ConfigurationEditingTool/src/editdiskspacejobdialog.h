#ifndef EDITDISKSPACEJOBDIALOG_H
#define EDITDISKSPACEJOBDIALOG_H

#include "abstracteditjobdialog.h"

namespace Ui {
   class EditDiskSpaceJobDialog;
}

class EditDiskSpaceJobDialog : public AbstractEditJobDialog
{
   Q_OBJECT

public:
   EditDiskSpaceJobDialog(AbstractJob* _job);
   virtual ~EditDiskSpaceJobDialog();

private slots:
   void on_buttonBox_accepted();

   void on_buttonBox_rejected();

private:
   void UpdateUiFromJob();
   void UpdateJobFromUi();

   Ui::EditDiskSpaceJobDialog *ui;
};

#endif // EDITDISKSPACEJOBDIALOG_H
