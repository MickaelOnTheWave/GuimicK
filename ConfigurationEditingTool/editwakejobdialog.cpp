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

void EditWakeJobDialog::on_okButton_clicked()
{
   accept();
}

void EditWakeJobDialog::on_cancelButton_clicked()
{
   reject();
}
