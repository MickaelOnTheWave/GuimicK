#include "wakejobdisplay.h"
#include "ui_wakejobdisplay.h"

#include "abstractwakejob.h"

WakeJobDisplay::WakeJobDisplay(QWidget *parent) :
   AbstractDisplay(parent),
   ui(new Ui::WakeJobDisplay)
{
   ui->setupUi(this);
}

WakeJobDisplay::~WakeJobDisplay()
{
   delete ui;
}

void WakeJobDisplay::Initialize(AbstractJob* job)
{
   auto wakeJob = dynamic_cast<AbstractWakeJob*>(job);
   if (wakeJob)
   {
      ui->nameLabel->setText(wakeJob->GetName().c_str());
      ui->typeLabel->setVisible((wakeJob->GetName() != "Wake"));
      ui->maxRetriesLabel->setText(QString::number(wakeJob->GetMaxRetries()));
      ui->timeoutLabel->setText(QString::number(wakeJob->GetTimeout()));
   }
}
