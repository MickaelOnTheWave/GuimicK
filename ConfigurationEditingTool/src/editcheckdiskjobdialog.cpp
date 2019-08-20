#include "editcheckdiskjobdialog.h"
#include "ui_editcheckdiskjobdialog.h"

#include <QButtonGroup>
#include <QDir>
#include <QFileDialog>
#include <QRadioButton>
#include "diskrelatedjob.h"

EditCheckDiskJobDialog::EditCheckDiskJobDialog(AbstractJob *_job) :
    AbstractEditJobDialog(_job),
    ui(new Ui::EditCheckDiskJobDialog)
{
    ui->setupUi(this);
    SetupUiWithDrives();
    UpdateUiFromJob();

    connect(ui->closeButtonBox, SIGNAL(accepted()), this, SLOT(OnCloseAccepting()));
    connect(ui->closeButtonBox, SIGNAL(rejected()), this, SLOT(OnCloseRejecting()));
}

EditCheckDiskJobDialog::~EditCheckDiskJobDialog()
{
    delete ui;
}

void EditCheckDiskJobDialog::UpdateUiFromJob()
{
    ui->jobnameEdit->setText(QString::fromStdWString(job->GetName()));

    auto castJob = dynamic_cast<DiskRelatedJob*>(job);
    const QString drive = QString::fromStdWString(castJob->GetDrive());
    if (drive != "")
       SelectDriveOnUi(drive);
}

void EditCheckDiskJobDialog::UpdateJobFromUi()
{
    job->SetName(ui->jobnameEdit->text().toStdWString());

    auto castJob = dynamic_cast<DiskRelatedJob*>(job);
    QAbstractButton* selectedDrive = driveGroup->checkedButton();
    const QString drive = (selectedDrive) ? selectedDrive->text() : "";
    castJob->SetDrive(drive.toStdWString());
}

void EditCheckDiskJobDialog::SetupUiWithDrives()
{
   AddDriveButtons(DetectDrives());
}

void EditCheckDiskJobDialog::SelectDriveOnUi(const QString& drive)
{
   QAbstractButton* driveButton = driveButtonMap[drive];
   driveButton->setChecked(true);
}

QStringList EditCheckDiskJobDialog::DetectDrives()
{
   QStringList driveNames;
   QFileInfoList drivesInfo = QDir::drives();
   for (const auto& driveInfo : drivesInfo)
      driveNames.push_back(driveInfo.filePath());
   return driveNames;
}

void EditCheckDiskJobDialog::AddDriveButtons(const QStringList& drives)
{
   driveGroup = new QButtonGroup();
   for (const QString& drive : drives)
   {
      auto button = new QRadioButton(drive);
      button->setIcon(QIcon(":/icons/hd_50"));
      driveGroup->addButton(button);
      ui->drivesLayout->addWidget(button);
      driveButtonMap[drive] = button;
   }
}
