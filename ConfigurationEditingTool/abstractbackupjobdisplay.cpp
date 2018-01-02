#include "abstractbackupjobdisplay.h"
#include "ui_abstractbackupjobdisplay.h"

#include "abstractbackupjob.h"

using namespace std;

AbstractBackupJobDisplay::AbstractBackupJobDisplay(QWidget *parent) :
   AbstractDisplay(parent),
   ui(new Ui::AbstractBackupJobDisplay)
{
   ui->setupUi(this);
}

AbstractBackupJobDisplay::~AbstractBackupJobDisplay()
{
   delete ui;
}

void AbstractBackupJobDisplay::Initialize(AbstractJob* job)
{
   auto backupJob = dynamic_cast<AbstractBackupJob*>(job);
   if (backupJob)
   {
      ui->titleLabel->setText(backupJob->GetName().c_str());
      ui->typeLabel->setText(backupJob->GetTypeName().c_str());

      vector<pair<string,string> > backupPoints;
      backupJob->GetFolderList(backupPoints);
      ui->mutipleBackupsLabel->setText(QString("%1 backup points configured").arg(backupPoints.size()));

      if (backupPoints.size() > 0)
      {
         ui->sourceLabel->setText(backupPoints.front().first.c_str());
         ui->destinationLabel->setText(backupPoints.front().second.c_str());
      }

      if (backupPoints.size() < 2)
         ui->backupInfoWidget->setCurrentIndex(backupPoints.size());
      else
         ui->backupInfoWidget->setCurrentIndex(2);
   }
}
