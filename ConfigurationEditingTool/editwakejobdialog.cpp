#include "editwakejobdialog.h"
#include "ui_editwakejobdialog.h"

EditWakeJobDialog::EditWakeJobDialog(QWidget *parent) :
   QDialog(parent),
   ui(new Ui::EditWakeJobDialog)
{
   ui->setupUi(this);
}

EditWakeJobDialog::~EditWakeJobDialog()
{
   delete ui;
}
