#ifndef EDITZIPCOPYBACKUPDIALOG_H
#define EDITZIPCOPYBACKUPDIALOG_H

#include "abstracteditjobdialog.h"
#include "editbackupjobwidget.h"

namespace Ui {
   class EditZipCopyBackupDialog;
}

class EditZipCopyBackupDialog : public AbstractEditJobDialog
{
   Q_OBJECT

public:
   explicit EditZipCopyBackupDialog(AbstractJob* _job);
   virtual ~EditZipCopyBackupDialog();

   void SetupFor(const ConfigurationType& type) override;

protected:
   virtual void UpdateUiFromJob() override;
   virtual void UpdateJobFromUi() override;

private:
   void CreateBasicBackupWidget();

private:
   Ui::EditZipCopyBackupDialog *ui;
   EditBackupJobWidget* basicBackupWidget;
};

#endif // EDITZIPCOPYBACKUPDIALOG_H
