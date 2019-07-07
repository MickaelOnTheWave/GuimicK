#include "adminrightswarning.h"
#include "ui_adminrightswarning.h"

AdminRightsWarning::AdminRightsWarning(QWidget *parent) :
   QFrame(parent),
   ui(new Ui::AdminRightsWarning)
{
   ui->setupUi(this);
}

AdminRightsWarning::~AdminRightsWarning()
{
   delete ui;
}
