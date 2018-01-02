#include "abstractbackupjobdisplay.h"
#include "ui_abstractbackupjobdisplay.h"

AbstractBackupJobDisplay::AbstractBackupJobDisplay(QWidget *parent) :
   QWidget(parent),
   ui(new Ui::AbstractBackupJobDisplay)
{
   ui->setupUi(this);
}

AbstractBackupJobDisplay::~AbstractBackupJobDisplay()
{
   delete ui;
}
