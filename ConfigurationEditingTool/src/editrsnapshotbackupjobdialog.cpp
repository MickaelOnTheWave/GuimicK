#include "editrsnapshotbackupjobdialog.h"
#include "ui_editrsnapshotbackupjobdialog.h"

#include <QVBoxLayout>
#include "rsnapshotsmartbackupjob.h"

EditRsnapshotBackupJobDialog::EditRsnapshotBackupJobDialog(AbstractJob* _job) :
   AbstractEditJobDialog(_job),
   ui(new Ui::EditRsnapshotBackupJobDialog)
{
   ui->setupUi(this);
   CreateBasicBackupWidget();
   UpdateUiFromJob();

   connect(ui->closeButtonBox, SIGNAL(accepted()), this, SLOT(OnCloseAccepting()));
   connect(ui->closeButtonBox, SIGNAL(rejected()), this, SLOT(OnCloseRejecting()));
}

EditRsnapshotBackupJobDialog::~EditRsnapshotBackupJobDialog()
{
   delete ui;
   delete basicBackupWidget;
}

void EditRsnapshotBackupJobDialog::UpdateUiFromJob()
{
   auto rsnapshotJob = static_cast<RsnapshotSmartBackupJob*>(job);
   basicBackupWidget->UpdateUiFromJob(rsnapshotJob);
   ui->repositoryEdit->setText(QString::fromStdWString(rsnapshotJob->GetRepository()));
   ui->templateConfFileEdit->setText(QString::fromStdWString(rsnapshotJob->GetTemplateConfigurationFile()));
   ui->tempConfFileEdit->setText(QString::fromStdWString(rsnapshotJob->GetTemporaryFile()));
   ui->maxBackupsSpinBox->setValue(rsnapshotJob->GetMaxBackupCount());
}

void EditRsnapshotBackupJobDialog::UpdateJobFromUi()
{
   auto rsnapshotJob = static_cast<RsnapshotSmartBackupJob*>(job);
   basicBackupWidget->UpdateJobFromUi(rsnapshotJob);
   rsnapshotJob->SetRepository(ui->repositoryEdit->text().toStdWString());
   rsnapshotJob->SetTemplateConfigurationFile(ui->templateConfFileEdit->text().toStdWString());
   rsnapshotJob->SetTemporaryFile(ui->tempConfFileEdit->text().toStdWString());
   rsnapshotJob->SetMaxBackupCount(ui->maxBackupsSpinBox->value());
}

void EditRsnapshotBackupJobDialog::CreateBasicBackupWidget()
{
   basicBackupWidget = new EditBackupJobWidget();

   auto layout = new QVBoxLayout();
   layout->addWidget(basicBackupWidget);
   ui->basicBox->setLayout(layout);
}
