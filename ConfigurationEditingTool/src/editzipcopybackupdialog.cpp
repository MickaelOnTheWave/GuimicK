#include "editzipcopybackupdialog.h"
#include "ui_editzipcopybackupdialog.h"

#include "zipandcopyfsbackupjob.h"

namespace
{
#ifdef _WIN32
   const QString fileSuffix = "zip";
#else
   const QString fileSuffix = "tar";
#endif
}

EditZipCopyBackupDialog::EditZipCopyBackupDialog(AbstractJob* _job) :
   AbstractEditJobDialog(_job),
   ui(new Ui::EditZipCopyBackupDialog)
{
   ui->setupUi(this);
   CreateBasicBackupWidget();
   UpdateUiFromJob();

   connect(ui->closeButtonBox, SIGNAL(accepted()), this, SLOT(OnCloseAccepting()));
   connect(ui->closeButtonBox, SIGNAL(rejected()), this, SLOT(OnCloseRejecting()));
}

EditZipCopyBackupDialog::~EditZipCopyBackupDialog()
{
   delete ui;
   delete basicBackupWidget;
}

void EditZipCopyBackupDialog::SetupFor(const ConfigurationType& type)
{
   if (type != ClientConfigurationType)
      ui->zipCopyBox->setVisible(false);
}

void EditZipCopyBackupDialog::UpdateUiFromJob()
{
   auto zipCopyJob = static_cast<ZipAndCopyFsBackupJob*>(job);
   basicBackupWidget->UpdateUiFromJob(zipCopyJob);
   ui->localDestinationEdit->setText(zipCopyJob->GetLocalDestination().c_str());
}

void EditZipCopyBackupDialog::UpdateJobFromUi()
{
   auto zipCopyJob = static_cast<ZipAndCopyFsBackupJob*>(job);
   basicBackupWidget->UpdateJobFromUi(zipCopyJob);
   zipCopyJob->SetLocalDestination(ui->localDestinationEdit->text().toStdString());
}

void EditZipCopyBackupDialog::CreateBasicBackupWidget()
{
   const QString fileFilter = "Archive files (*." + fileSuffix + ")";
   basicBackupWidget = new EditBackupJobWidget();
   basicBackupWidget->SetupDestinationAsFile("Choose Destination Archive",
                                             fileFilter);

   auto layout = new QVBoxLayout();
   layout->addWidget(basicBackupWidget);
   ui->basicBox->setLayout(layout);
}
