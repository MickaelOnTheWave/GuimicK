#include "editrsyncfsbackupjobdialog.h"
#include "ui_editrsyncfsbackupjobdialog.h"

#include "rsynccopyfsbackupjob.h"

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

}

void EditRsyncFsBackupJobDialog::UpdateJobFromUi()
{
   auto rsyncJob = static_cast<RsyncCopyFsBackupJob*>(job);
   basicBackupWidget->UpdateJobFromUi(rsyncJob);

}

void EditRsyncFsBackupJobDialog::CreateBasicBackupWidget()
{
   basicBackupWidget = new EditBackupJobWidget();

   auto layout = new QVBoxLayout();
   layout->addWidget(basicBackupWidget);
   ui->basicBox->setLayout(layout);
}
