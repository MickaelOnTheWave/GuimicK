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
      ui->titleLabel->setText(QString::fromStdWString(backupJob->GetName()));
      ui->typeLabel->setText(QString::fromStdWString(backupJob->GetTypeName()));

      vector<pair<wstring,wstring> > backupPoints;
      backupJob->GetFolderList(backupPoints);
      ui->mutipleBackupsLabel->setText(QString("%1 backup points configured").arg(backupPoints.size()));

      if (backupPoints.size() > 0)
      {
         ui->sourceLabel->setText(QString::fromStdWString(backupPoints.front().first));
         ui->destinationLabel->setText(QString::fromStdWString(backupPoints.front().second));
      }

      if (backupPoints.size() < 2)
         ui->backupInfoWidget->setCurrentIndex(static_cast<int>(backupPoints.size()));
      else
         ui->backupInfoWidget->setCurrentIndex(2);
   }
}
