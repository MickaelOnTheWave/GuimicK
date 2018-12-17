#include "editbackupjobwidget.h"
#include "ui_editbackupjobwidget.h"

using namespace std;

EditBackupJobWidget::EditBackupJobWidget(QWidget *parent) :
   QWidget(parent),
   ui(new Ui::EditBackupJobWidget)
{
   ui->setupUi(this);
   InitializeControls();
   EnableControls(false);
}

EditBackupJobWidget::~EditBackupJobWidget()
{
   delete ui;
}

void EditBackupJobWidget::SetupDestinationAsFile(const QString& message,
                                                 const QString& fileFilter)
{
   ui->destinationWidget->InitializeAsFile("Destination", message, "", fileFilter);
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
   ui->backupPointsWidget->selectRow(addIndex);
}

void EditBackupJobWidget::on_removeBackupPointButton_clicked()
{
   ui->backupPointsWidget->removeRow(ui->backupPointsWidget->currentRow());
}

void EditBackupJobWidget::on_backupPointsWidget_itemSelectionChanged()
{
   const int currentRowIndex = ui->backupPointsWidget->currentRow();
   const bool isIndexValid = (currentRowIndex != -1);
   EnableControls(isIndexValid);
   if (isIndexValid)
   {
      SetFolderWidgetValue(ui->sourceWidget, currentRowIndex, 0);
      SetFolderWidgetValue(ui->destinationWidget, currentRowIndex, 1);
   }
}

void EditBackupJobWidget::OnFinishedSourceEditing(const QString& value)
{
   const int currentRow = ui->backupPointsWidget->currentRow();
   if (currentRow != -1)
      ui->backupPointsWidget->setItem(currentRow, 0, new QTableWidgetItem(value));
}

void EditBackupJobWidget::OnFinishedDestinationEditing(const QString& value)
{
   const int currentRow = ui->backupPointsWidget->currentRow();
   if (currentRow != -1)
      ui->backupPointsWidget->setItem(currentRow, 1, new QTableWidgetItem(value));
}

void EditBackupJobWidget::SetFolderWidgetValue(
      PathSelectionControl* widget,
      const int rowIndex, const int columnIndex)
{
   QTableWidgetItem* tableItem = ui->backupPointsWidget->item(rowIndex, columnIndex);
   const QString value = (tableItem) ? tableItem->text() : QString("");
   widget->SetPath(value);
}

void EditBackupJobWidget::InitializeControls()
{
   ui->sourceWidget->InitializeAsFolder("Source", "Select folder to backup", "");
   connect(ui->sourceWidget, SIGNAL(PathChanged(QString)),
           this, SLOT(OnFinishedSourceEditing(QString)));
   ui->destinationWidget->InitializeAsFolder("Destination", "Select backup destination", "");
   connect(ui->destinationWidget, SIGNAL(PathChanged(QString)),
           this, SLOT(OnFinishedDestinationEditing(QString)));

   QHeaderView* header = ui->backupPointsWidget->horizontalHeader();
   header->setSectionResizeMode(QHeaderView::Stretch);
}

void EditBackupJobWidget::EnableControls(const bool value)
{
   ui->removeBackupPointButton->setEnabled(value);
   ui->sourceWidget->Enable(value);
   ui->destinationWidget->Enable(value);
}

