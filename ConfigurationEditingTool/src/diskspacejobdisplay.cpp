#include "diskspacejobdisplay.h"
#include "ui_diskspacejobdisplay.h"

#include "linuxfreespacecheckjob.h"
#include "windowscheckdiskjob.h"

DiskSpaceJobDisplay::DiskSpaceJobDisplay(QWidget *parent) :
   AbstractDisplay(parent),
   ui(new Ui::DiskSpaceJobDisplay)
{
   ui->setupUi(this);
}

DiskSpaceJobDisplay::~DiskSpaceJobDisplay()
{
   delete ui;
}

void DiskSpaceJobDisplay::Initialize(AbstractJob* job)
{
   auto diskSpaceJob = dynamic_cast<DiskRelatedJob*>(job);
   if (diskSpaceJob)
   {
      const std::wstring jobTypeName = GetDefaultTypeName(job);
      ui->jobNameLabel->setText(QString::fromStdWString(diskSpaceJob->GetName()));
      ui->jobTypeLabel->setText(QString::fromStdWString(jobTypeName));
      ui->jobTypeLabel->setVisible((diskSpaceJob->GetName() != jobTypeName));
      ui->driveLabel->setText(CreateDriveLabel(diskSpaceJob->GetDrive()));
   }
}

std::wstring DiskSpaceJobDisplay::GetDefaultTypeName(AbstractJob* job)
{
   if (dynamic_cast<LinuxFreeSpaceCheckJob*>(job))
      return L"Free space checking";
   else if (dynamic_cast<WindowsCheckdiskJob*>(job))
      return L"HD Integrity Check";
   else
      return L"Unknown type";
}

QString DiskSpaceJobDisplay::CreateDriveLabel(const std::wstring& driveName)
{
   return (driveName == L"") ? QString("no drive") : QString::fromStdWString(driveName);
}
