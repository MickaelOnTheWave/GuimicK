#include "abstractjobdisplay.h"
#include "ui_abstractjobdisplay.h"

#include "debugoutput.h"

AbstractJobDisplay::AbstractJobDisplay(QWidget *parent) :
   AbstractDisplay(parent),
   ui(new Ui::AbstractJobDisplay)
{
   ui->setupUi(this);
}

AbstractJobDisplay::~AbstractJobDisplay()
{
   delete ui;
}

void AbstractJobDisplay::Initialize(AbstractJob* job)
{
   ui->jobnameLabel->setText(QString::fromStdWString(job->GetName()));

   const int debugValue = job->GetOutputDebugInformationValue();
   ui->debugValueLabel->setText(QString::fromStdWString(DebugOutput::GetDescription(debugValue)));
}
