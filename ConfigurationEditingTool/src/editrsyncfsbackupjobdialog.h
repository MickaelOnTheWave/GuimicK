#ifndef EDITRSYNCFSBACKUPJOBDIALOG_H
#define EDITRSYNCFSBACKUPJOBDIALOG_H

#include "abstracteditjobdialog.h"
#include "editbackupjobwidget.h"

namespace Ui {
   class EditRsyncFsBackupJobDialog;
}

class EditRsyncFsBackupJobDialog : public AbstractEditJobDialog
{
   Q_OBJECT

public:
   explicit EditRsyncFsBackupJobDialog(AbstractJob* _job);
   virtual ~EditRsyncFsBackupJobDialog();

protected:
   virtual void UpdateUiFromJob() override;
   virtual void UpdateJobFromUi() override;

private slots:
   void on_addExclusionButton_clicked();

   void on_removeExclusionButton_clicked();

private:
   void CreateBasicBackupWidget();
   void UpdateUiExclusions();
   void UpdateJobExclusions();

private:
   Ui::EditRsyncFsBackupJobDialog *ui;
   EditBackupJobWidget* basicBackupWidget;
};

#endif // EDITRSYNCFSBACKUPJOBDIALOG_H
