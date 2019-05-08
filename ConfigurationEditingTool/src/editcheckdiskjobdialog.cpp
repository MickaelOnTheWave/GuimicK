#include "editcheckdiskjobdialog.h"
#include "ui_editcheckdiskjobdialog.h"

#include <QDir>
#include <QFileDialog>
#include "diskrelatedjob.h"

namespace
{
    const QString uiNoDrive = "none";

    QString GetDrive(const QString& path)
    {
        return path.section('/', 0, 0);
    }
}

EditCheckDiskJobDialog::EditCheckDiskJobDialog(AbstractJob *_job) :
    AbstractEditJobDialog(_job),
    ui(new Ui::EditCheckDiskJobDialog)
{
    ui->setupUi(this);
    UpdateUiFromJob();

    connect(ui->closeButtonBox, SIGNAL(accepted()), this, SLOT(OnCloseAccepting()));
    connect(ui->closeButtonBox, SIGNAL(rejected()), this, SLOT(OnCloseRejecting()));
}

EditCheckDiskJobDialog::~EditCheckDiskJobDialog()
{
    delete ui;
}

void EditCheckDiskJobDialog::on_chooseDiskButton_pressed()
{
    // TODO : setup a real widget based on this to select the drive
    /*for (const QFileInfo& device : QDir::drives())
    {
        auto temp = device.baseName();
    }*/


    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly, true);
    if (dialog.exec() == QDialog::Accepted)
        ui->selectedDiskLabel->setText(GetDrive(dialog.selectedFiles().first()));
}

void EditCheckDiskJobDialog::UpdateUiFromJob()
{
    ui->jobnameEdit->setText(QString::fromStdWString(job->GetName()));

    auto castJob = dynamic_cast<DiskRelatedJob*>(job);
    QString drive = QString::fromStdWString(castJob->GetDrive());
    if (drive == "")
        drive = uiNoDrive;
    ui->selectedDiskLabel->setText(drive);
}

void EditCheckDiskJobDialog::UpdateJobFromUi()
{
    job->SetName(ui->jobnameEdit->text().toStdWString());

    auto castJob = dynamic_cast<DiskRelatedJob*>(job);
    QString drive = ui->selectedDiskLabel->text();
    if (drive == uiNoDrive)
        drive = "";
    castJob->SetDrive(drive.toStdWString());
}
