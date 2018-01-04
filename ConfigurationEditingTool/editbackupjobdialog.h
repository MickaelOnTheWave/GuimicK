#ifndef EDITBACKUPJOBDIALOG_H
#define EDITBACKUPJOBDIALOG_H

#include "abstracteditjobdialog.h"
#include "editbackupjobwidget.h"

namespace Ui {
   class EditBackupJobDialog;
}

class EditBackupJobDialog : public AbstractEditJobDialog
{
   Q_OBJECT

public:
   explicit EditBackupJobDialog(AbstractJob* _job);
   virtual ~EditBackupJobDialog();

protected:
   virtual void UpdateUiFromJob() override;
   virtual void UpdateJobFromUi() override;

private:
   void CreateBasicBackupWidget();

   Ui::EditBackupJobDialog *ui;
   EditBackupJobWidget* basicBackupWidget;
};

#endif // EDITBACKUPJOBDIALOG_H
