#ifndef EDITBACKUPJOBDIALOG_H
#define EDITBACKUPJOBDIALOG_H

#include "abstracteditjobdialog.h"

namespace Ui {
   class EditBackupJobDialog;
}

class EditBackupJobDialog : public AbstractEditJobDialog
{
   Q_OBJECT

public:
   explicit EditBackupJobDialog(AbstractJob* _job);
   virtual ~EditBackupJobDialog();

private slots:
   void on_buttonBox_accepted();

   void on_buttonBox_rejected();

   void on_addBackupPointButton_clicked();

   void on_removeBackupPointButton_clicked();

   void on_backupPointsWidget_itemSelectionChanged();

private:
   void UpdateUiFromJob();
   void UpdateJobFromUi();

   void AddBackupPointsToUi(
      const std::vector<std::pair<std::string,std::string> >& backupPoints
   );
   void AddBackupPointsToJob();

   Ui::EditBackupJobDialog *ui;
};

#endif // EDITBACKUPJOBDIALOG_H
