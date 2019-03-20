#include "selectbackupfolderdialog.h"
#include "ui_selectbackupfolderdialog.h"

using namespace std;

SelectBackupFolderDialog::SelectBackupFolderDialog(AbstractBackupJob* job) :
   QDialog(),
   ui(new Ui::SelectBackupFolderDialog),
   selectedBackup("")
{
   ui->setupUi(this);

   FillUiFromJob(job);
}

SelectBackupFolderDialog::~SelectBackupFolderDialog()
{
   delete ui;
}

QString SelectBackupFolderDialog::GetSelectedBackup() const
{
   return selectedBackup;
}

void SelectBackupFolderDialog::on_backupWidget_clicked(const QModelIndex &index)
{
   selectedBackup = index.data().toString();
}

void SelectBackupFolderDialog::FillUiFromJob(AbstractBackupJob* job)
{
   vector<pair<wstring,wstring> > backedUpFolders;
   job->GetFolderList(backedUpFolders);

   for (auto it : backedUpFolders)
      ui->backupWidget->addItem(QString::fromStdWString(it.first));
}
