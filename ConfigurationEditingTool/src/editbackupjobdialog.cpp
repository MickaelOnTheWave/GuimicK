#include "editbackupjobdialog.h"
#include "ui_editbackupjobdialog.h"

#include <QVBoxLayout>
#include "abstractbackupjob.h"

using namespace std;

EditBackupJobDialog::EditBackupJobDialog(AbstractJob* _job) :
   AbstractEditJobDialog(_job),
   ui(new Ui::EditBackupJobDialog)
{
   ui->setupUi(this);
   CreateBasicBackupWidget();
   UpdateUiFromJob();

   connect(ui->closeButtonBox, SIGNAL(accepted()), this, SLOT(OnCloseAccepting()));
   connect(ui->closeButtonBox, SIGNAL(rejected()), this, SLOT(OnCloseRejecting()));
}

EditBackupJobDialog::~EditBackupJobDialog()
{
   delete ui;
   delete basicBackupWidget;
}

void EditBackupJobDialog::UpdateUiFromJob()
{
   auto backupJob = static_cast<AbstractBackupJob*>(job);
   basicBackupWidget->UpdateUiFromJob(backupJob);
}

void EditBackupJobDialog::UpdateJobFromUi()
{
   auto backupJob = static_cast<AbstractBackupJob*>(job);
   basicBackupWidget->UpdateJobFromUi(backupJob);
}

void EditBackupJobDialog::CreateBasicBackupWidget()
{
   basicBackupWidget = new EditBackupJobWidget();
   auto mainLayout = static_cast<QVBoxLayout*>(layout());
   mainLayout->insertWidget(0, basicBackupWidget);
}
