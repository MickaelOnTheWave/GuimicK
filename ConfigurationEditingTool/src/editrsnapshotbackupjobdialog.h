#ifndef EDITRSNAPSHOTBACKUPJOBDIALOG_H
#define EDITRSNAPSHOTBACKUPJOBDIALOG_H

#include "abstracteditjobdialog.h"
#include "editbackupjobwidget.h"

namespace Ui {
   class EditRsnapshotBackupJobDialog;
}

class EditRsnapshotBackupJobDialog : public AbstractEditJobDialog
{
   Q_OBJECT

public:
   explicit EditRsnapshotBackupJobDialog(AbstractJob* _job);
   virtual ~EditRsnapshotBackupJobDialog();

protected:
   virtual void UpdateUiFromJob() override;
   virtual void UpdateJobFromUi() override;

private:
   void CreateBasicBackupWidget();

   Ui::EditRsnapshotBackupJobDialog *ui;
   EditBackupJobWidget* basicBackupWidget;
};

#endif // EDITRSNAPSHOTBACKUPJOBDIALOG_H
