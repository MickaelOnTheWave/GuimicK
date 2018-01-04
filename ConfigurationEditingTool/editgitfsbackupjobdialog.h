#ifndef EDITGITFSBACKUPJOBDIALOG_H
#define EDITGITFSBACKUPJOBDIALOG_H

#include "abstracteditjobdialog.h"
#include "editbackupjobwidget.h"

namespace Ui {
   class EditGitFsBackupJobDialog;
}

class EditGitFsBackupJobDialog : public AbstractEditJobDialog
{
   Q_OBJECT

public:
   explicit EditGitFsBackupJobDialog(AbstractJob* _job);
   virtual ~EditGitFsBackupJobDialog();

protected:
   virtual void UpdateUiFromJob() override;
   virtual void UpdateJobFromUi() override;

private:
   void CreateBasicBackupWidget();

private:
   Ui::EditGitFsBackupJobDialog *ui;
   EditBackupJobWidget* basicBackupWidget;
};

#endif // EDITGITFSBACKUPJOBDIALOG_H
