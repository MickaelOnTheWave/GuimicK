#include "editbackupjobdialog.h"
#include "ui_editbackupjobdialog.h"

#include "abstractbackupjob.h"

using namespace std;

EditBackupJobDialog::EditBackupJobDialog(AbstractJob* _job) :
   AbstractEditJobDialog(_job),
   ui(new Ui::EditBackupJobDialog)
{
   ui->setupUi(this);
   UpdateUiFromJob();
}

EditBackupJobDialog::~EditBackupJobDialog()
{
   delete ui;
}

void EditBackupJobDialog::on_buttonBox_accepted()
{
   UpdateJobFromUi();
   accept();
}

void EditBackupJobDialog::on_buttonBox_rejected()
{
   reject();
}

void EditBackupJobDialog::UpdateUiFromJob()
{
   auto backupJob = dynamic_cast<AbstractBackupJob*>(job);
   if (!backupJob)
      return;

   vector<pair<string,string> > backupPoints;
   backupJob->GetFolderList(backupPoints);
   AddBackupPointsToUi(backupPoints);
}

void EditBackupJobDialog::UpdateJobFromUi()
{
}

void EditBackupJobDialog::AddBackupPointsToUi(
   const vector<pair<string, string> >& backupPoints
)
{
   ui->backupPointsWidget->setRowCount(backupPoints.size());
   int index = 0;
   for (const auto it : backupPoints)
   {
      ui->backupPointsWidget->setItem(index, 0, new QTableWidgetItem(it.first.c_str()));
      ui->backupPointsWidget->setItem(index, 1, new QTableWidgetItem(it.second.c_str()));
      ++index;
   }
   ui->backupPointsWidget->resizeColumnsToContents();
}

void EditBackupJobDialog::on_addBackupPointButton_clicked()
{
   ui->backupPointsWidget->insertRow(ui->backupPointsWidget->currentRow());
}

void EditBackupJobDialog::on_removeBackupPointButton_clicked()
{
   ui->backupPointsWidget->removeRow(ui->backupPointsWidget->currentRow());
}
