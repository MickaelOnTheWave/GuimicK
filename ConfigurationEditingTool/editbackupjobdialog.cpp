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
   auto backupJob = static_cast<AbstractBackupJob*>(job);
   ui->jobNameEdit->setText(backupJob->GetName().c_str());

   vector<pair<string,string> > backupPoints;
   backupJob->GetFolderList(backupPoints);
   AddBackupPointsToUi(backupPoints);
   ui->removeBackupPointButton->setEnabled(false);
}

void EditBackupJobDialog::UpdateJobFromUi()
{
   auto backupJob = static_cast<AbstractBackupJob*>(job);
   backupJob->SetName(ui->jobNameEdit->text().toStdString());
   AddBackupPointsToJob();
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

void EditBackupJobDialog::AddBackupPointsToJob()
{
   auto backupJob = static_cast<AbstractBackupJob*>(job);
   backupJob->ClearFolderList();
   for (int i=0; i<ui->backupPointsWidget->rowCount(); ++i)
   {
      QTableWidgetItem* sourceItem = ui->backupPointsWidget->item(i, 0);
      QTableWidgetItem* destinationItem = ui->backupPointsWidget->item(i, 1);
      backupJob->AddFolder(sourceItem->text().toStdString(), destinationItem->text().toStdString());
   }
}

void EditBackupJobDialog::on_addBackupPointButton_clicked()
{
   const int currentIndex = ui->backupPointsWidget->currentRow();
   const int addIndex = (currentIndex == -1) ? ui->backupPointsWidget->rowCount() : currentIndex+1;
   ui->backupPointsWidget->insertRow(addIndex);
}

void EditBackupJobDialog::on_removeBackupPointButton_clicked()
{
   ui->backupPointsWidget->removeRow(ui->backupPointsWidget->currentRow());
}

void EditBackupJobDialog::on_backupPointsWidget_itemSelectionChanged()
{
   ui->removeBackupPointButton->setEnabled(ui->backupPointsWidget->currentRow() != -1);
}
