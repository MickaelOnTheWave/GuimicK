#include "tasktoolrundialog.h"
#include "ui_tasktoolrundialog.h"

TaskToolRunDialog::TaskToolRunDialog(QWidget *parent) :
   QDialog(parent),
   ui(new Ui::TaskToolRunDialog)
{
   ui->setupUi(this);
}

TaskToolRunDialog::~TaskToolRunDialog()
{
   delete ui;
}
