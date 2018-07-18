#include "editbackupjobwidget.h"
#include "ui_editbackupjobwidget.h"

using namespace std;

EditBackupJobWidget::EditBackupJobWidget(QWidget *parent) :
   QWidget(parent),
   ui(new Ui::EditBackupJobWidget)
{
   ui->setupUi(this);
}

EditBackupJobWidget::~EditBackupJobWidget()
{
   delete ui;
}

void EditBackupJobWidget::UpdateUiFromJob(AbstractBackupJob* job)
{
   auto backupJob = static_cast<AbstractBackupJob*>(job);
   ui->jobNameEdit->setText(backupJob->GetName().c_str());

   vector<pair<string,string> > backupPoints;
   backupJob->GetFolderList(backupPoints);
   AddBackupPointsToUi(backupPoints);
   ui->removeBackupPointButton->setEnabled(false);
}

void EditBackupJobWidget::UpdateJobFromUi(AbstractBackupJob* job) const
{
   auto backupJob = static_cast<AbstractBackupJob*>(job);
   backupJob->SetName(ui->jobNameEdit->text().toStdString());
   AddBackupPointsToJob(job);
}

void EditBackupJobWidget::AddBackupPointsToUi(
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

void EditBackupJobWidget::AddBackupPointsToJob(AbstractBackupJob* job) const
{
   auto backupJob = static_cast<AbstractBackupJob*>(job);
   backupJob->ClearFolderList();
   for (int i=0; i<ui->backupPointsWidget->rowCount(); ++i)
   {
      QTableWidgetItem* sourceItem = ui->backupPointsWidget->item(i, 0);
      QTableWidgetItem* destinationItem = ui->backupPointsWidget->item(i, 1);
      const QString sourceContent = (sourceItem) ? sourceItem->text() : QString("");
      const QString destinationContent = (destinationItem) ? destinationItem->text() : QString("");
      backupJob->AddFolder(sourceContent.toStdString(), destinationContent.toStdString());
   }
}

void EditBackupJobWidget::on_addBackupPointButton_clicked()
{
   const int currentIndex = ui->backupPointsWidget->currentRow();
   const int addIndex = (currentIndex == -1) ? ui->backupPointsWidget->rowCount() : currentIndex+1;
   ui->backupPointsWidget->insertRow(addIndex);
}

void EditBackupJobWidget::on_removeBackupPointButton_clicked()
{
   ui->backupPointsWidget->removeRow(ui->backupPointsWidget->currentRow());
}

void EditBackupJobWidget::on_backupPointsWidget_itemSelectionChanged()
{
   ui->removeBackupPointButton->setEnabled(ui->backupPointsWidget->currentRow() != -1);
}
