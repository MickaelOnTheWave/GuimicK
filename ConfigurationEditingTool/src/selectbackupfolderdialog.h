#ifndef SELECTBACKUPFOLDERDIALOG_H
#define SELECTBACKUPFOLDERDIALOG_H

#include <QDialog>

#include "abstractbackupjob.h"

namespace Ui {
   class SelectBackupFolderDialog;
}

class SelectBackupFolderDialog : public QDialog
{
   Q_OBJECT

public:
   explicit SelectBackupFolderDialog(AbstractBackupJob* job);
   ~SelectBackupFolderDialog();

   QString GetSelectedBackup() const;

private slots:
   void on_backupWidget_clicked(const QModelIndex &index);

private:
   Ui::SelectBackupFolderDialog *ui;

   void FillUiFromJob(AbstractBackupJob* job);

   QString selectedBackup;
};

#endif // SELECTBACKUPFOLDERDIALOG_H
