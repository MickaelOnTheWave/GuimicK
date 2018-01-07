#include "editrsyncfsbackupjobdialog.h"
#include "ui_editrsyncfsbackupjobdialog.h"

#include "rsynccopyfsbackupjob.h"

using namespace std;

EditRsyncFsBackupJobDialog::EditRsyncFsBackupJobDialog(AbstractJob* _job) :
   AbstractEditJobDialog(_job),
   ui(new Ui::EditRsyncFsBackupJobDialog)
{
   ui->setupUi(this);
   CreateBasicBackupWidget();
   UpdateUiFromJob();

   connect(ui->closeButtonBox, SIGNAL(accepted()), this, SLOT(OnCloseAccepting()));
   connect(ui->closeButtonBox, SIGNAL(rejected()), this, SLOT(OnCloseRejecting()));
}

EditRsyncFsBackupJobDialog::~EditRsyncFsBackupJobDialog()
{
   delete ui;
   delete basicBackupWidget;
}

void EditRsyncFsBackupJobDialog::UpdateUiFromJob()
{
   auto rsyncJob = static_cast<RsyncCopyFsBackupJob*>(job);
   basicBackupWidget->UpdateUiFromJob(rsyncJob);
   UpdateUiExclusions();
}

void EditRsyncFsBackupJobDialog::UpdateJobFromUi()
{
   auto rsyncJob = static_cast<RsyncCopyFsBackupJob*>(job);
   basicBackupWidget->UpdateJobFromUi(rsyncJob);
   UpdateJobExclusions();
}

void EditRsyncFsBackupJobDialog::CreateBasicBackupWidget()
{
   basicBackupWidget = new EditBackupJobWidget();
   auto layout = new QVBoxLayout();
   layout->addWidget(basicBackupWidget);
   ui->basicTab->setLayout(layout);
}

void EditRsyncFsBackupJobDialog::UpdateUiExclusions()
{
   auto rsyncJob = static_cast<RsyncCopyFsBackupJob*>(job);
   vector<string> exclusions;
   rsyncJob->GetExclusionList(exclusions);

   for (auto it : exclusions)
      ui->exclusionListWidget->addItem(it.c_str());
}

void EditRsyncFsBackupJobDialog::UpdateJobExclusions()
{
   auto rsyncJob = static_cast<RsyncCopyFsBackupJob*>(job);
   rsyncJob->ClearExclusionList();
   for (int i=0; i<ui->exclusionListWidget->count(); ++i)
      rsyncJob->AddToExclusions(ui->exclusionListWidget->item(i)->text().toStdString());
}

void EditRsyncFsBackupJobDialog::on_addExclusionButton_clicked()
{
   auto newItem = new QListWidgetItem("New Exclusion");
   newItem->setFlags(newItem->flags () | Qt::ItemIsEditable);
   ui->exclusionListWidget->addItem(newItem);
}

void EditRsyncFsBackupJobDialog::on_removeExclusionButton_clicked()
{
    if (ui->exclusionListWidget->currentRow() == -1)
       return;

   auto item = ui->exclusionListWidget->takeItem(ui->exclusionListWidget->currentRow());
   delete item;
}
