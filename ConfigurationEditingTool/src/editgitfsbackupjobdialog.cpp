#include "editgitfsbackupjobdialog.h"
#include "ui_editgitfsbackupjobdialog.h"

#include "gitfsbackupjob.h"

EditGitFsBackupJobDialog::EditGitFsBackupJobDialog(AbstractJob* _job) :
   AbstractEditJobDialog(_job),
   ui(new Ui::EditGitFsBackupJobDialog)
{
   ui->setupUi(this);
   CreateBasicBackupWidget();
   UpdateUiFromJob();

   connect(ui->closeButtonBox, SIGNAL(accepted()), this, SLOT(OnCloseAccepting()));
   connect(ui->closeButtonBox, SIGNAL(rejected()), this, SLOT(OnCloseRejecting()));
}

EditGitFsBackupJobDialog::~EditGitFsBackupJobDialog()
{
   delete ui;
   delete basicBackupWidget;
}

void EditGitFsBackupJobDialog::UpdateUiFromJob()
{
   auto gitJob = static_cast<GitFsBackupJob*>(job);
   basicBackupWidget->UpdateUiFromJob(gitJob);

   ui->rawCopyBox->setChecked(gitJob->GetForceRawCopyUse());
}

void EditGitFsBackupJobDialog::UpdateJobFromUi()
{
   auto gitJob = static_cast<GitFsBackupJob*>(job);
   basicBackupWidget->UpdateJobFromUi(gitJob);

   gitJob->SetForceRawCopyUse(ui->rawCopyBox->isChecked());
}

void EditGitFsBackupJobDialog::CreateBasicBackupWidget()
{
   basicBackupWidget = new EditBackupJobWidget();

   auto layout = new QVBoxLayout();
   layout->addWidget(basicBackupWidget);
   ui->basicBox->setLayout(layout);
}
