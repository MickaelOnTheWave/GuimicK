#include "editbackupjobwidget.h"
#include "ui_editbackupjobwidget.h"
#include "destinationpathvalidator.h"

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
   ui->jobNameEdit->setText(QString::fromStdWString(backupJob->GetName()));

   vector<pair<wstring,wstring> > backupPoints;
   backupJob->GetFolderList(backupPoints);
   AddBackupPointsToUi(backupPoints);
   ui->removeBackupPointButton->setEnabled(false);
}

void EditBackupJobWidget::UpdateJobFromUi(AbstractBackupJob* job) const
{
   auto backupJob = static_cast<AbstractBackupJob*>(job);
   backupJob->SetName(ui->jobNameEdit->text().toStdWString());
   AddBackupPointsToJob(job);
}

void EditBackupJobWidget::AddBackupPointsToUi(
   const vector<pair<wstring, wstring> >& backupPoints
)
{
   ui->backupPointsWidget->setRowCount(static_cast<int>(backupPoints.size()));
   int index = 0;
   for (const auto& it : backupPoints)
   {
      ui->backupPointsWidget->setItem(index, 0, new QTableWidgetItem(QString::fromStdWString(it.first)));
      ui->backupPointsWidget->setItem(index, 1, new QTableWidgetItem(QString::fromStdWString(it.second)));
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
      backupJob->AddFolder(sourceContent.toStdWString(), destinationContent.toStdWString());
   }
}

QString EditBackupJobWidget::GetTableValue(const int rowIndex, const int columnIndex) const
{
   QTableWidgetItem* tableItem = ui->backupPointsWidget->item(rowIndex, columnIndex);
   return (tableItem) ? tableItem->text() : QString("");
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
      const QString value = GetTableValue(currentRowIndex, 0);
      ui->sourceWidget->SetPath(value);
      emit ui->sourceWidget->PathChanged(value);

      SetFolderWidgetValue(ui->destinationWidget, currentRowIndex, 1);
   }
}

void EditBackupJobWidget::OnFinishedSourceEditing(const QString& value)
{
   OnFinishedPathEditing(value, 0);
}

void EditBackupJobWidget::OnFinishedDestinationEditing(const QString& value)
{
   ui->destinationWidget->setStyleSheet("color: black;");
   ui->destinationWidget->setToolTip("");
   FinishedDestinationEditing(value);
}

void EditBackupJobWidget::OnValidDestination()
{
   ui->destinationWidget->setStyleSheet("color: black;");
   ui->destinationWidget->setToolTip("");
}

void EditBackupJobWidget::OnInvalidDestination(const QString& message)
{
   ui->destinationWidget->setStyleSheet("color: red;");
   ui->destinationWidget->setToolTip(message);
}

void EditBackupJobWidget::SetFolderWidgetValue(
      PathSelectionControl* widget,
      const int rowIndex, const int columnIndex)
{
   const QString value = GetTableValue(rowIndex, columnIndex);
   widget->SetPath(value);
}

void EditBackupJobWidget::InitializeControls()
{
   ui->sourceWidget->InitializeAsFolder("Source", "Select folder to backup", "");
   connect(ui->sourceWidget, SIGNAL(PathChanged(QString)),
           this, SLOT(OnFinishedSourceEditing(QString)));

   InitializeDestinationWidget();

   QHeaderView* header = ui->backupPointsWidget->horizontalHeader();
   header->setSectionResizeMode(QHeaderView::Stretch);
}

void EditBackupJobWidget::InitializeDestinationWidget()
{
   ui->destinationWidget->InitializeAsFolder("Destination", "Select backup destination", "");

   auto destinationValidator = new DestinationPathValidator(ui->sourceWidget->GetPath());
   ui->destinationWidget->SetValidator(destinationValidator);

   connect(ui->sourceWidget, SIGNAL(PathChanged(QString)), destinationValidator, SLOT(UpdateSourcePath(QString)));
   connect(ui->destinationWidget, SIGNAL(PathChanged(QString)), this, SLOT(OnFinishedDestinationEditing(QString)));

   connect(destinationValidator, SIGNAL(ValidDestination()), this, SLOT(OnValidDestination()));
   connect(destinationValidator, SIGNAL(Error(QString)), this, SLOT(OnInvalidDestination(QString)));
}

void EditBackupJobWidget::EnableControls(const bool value)
{
   ui->removeBackupPointButton->setEnabled(value);
   ui->sourceWidget->Enable(value);
   ui->destinationWidget->Enable(value);
}

void EditBackupJobWidget::OnFinishedPathEditing(const QString& value,
                                                const int columnIndex)
{
   const int currentRow = ui->backupPointsWidget->currentRow();
   if (currentRow != -1)
      ui->backupPointsWidget->setItem(currentRow, columnIndex, new QTableWidgetItem(value));
}

void EditBackupJobWidget::FinishedDestinationEditing(const QString& value)
{
   OnFinishedPathEditing(value, 1);
}

void EditBackupJobWidget::SetDestinationWidgetPath(const QString& value)
{
   // Used to prevent the change from firing validator and change recursively
   ui->destinationWidget->blockSignals(true);
   ui->destinationWidget->SetPath(value);
   ui->destinationWidget->blockSignals(false);
}
