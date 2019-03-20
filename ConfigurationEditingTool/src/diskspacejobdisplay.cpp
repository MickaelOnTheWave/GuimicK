#include "diskspacejobdisplay.h"
#include "ui_diskspacejobdisplay.h"

#include "linuxfreespacecheckjob.h"

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
   auto diskSpaceJob = dynamic_cast<LinuxFreeSpaceCheckJob*>(job);
   if (diskSpaceJob)
   {
      ui->jobNameLabel->setText(QString::fromStdWString(diskSpaceJob->GetName()));
      ui->jobTypeLabel->setVisible((diskSpaceJob->GetName() != L"Free space checking"));
      ui->driveLabel->setText(QString::fromStdWString(diskSpaceJob->GetDrive()));
   }
}
