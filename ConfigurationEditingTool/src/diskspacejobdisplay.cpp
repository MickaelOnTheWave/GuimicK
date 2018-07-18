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
      ui->jobNameLabel->setText(diskSpaceJob->GetName().c_str());
      ui->jobTypeLabel->setVisible((diskSpaceJob->GetName() != "Free space checking"));
      ui->driveLabel->setText(diskSpaceJob->GetDrive().c_str());
   }
}
