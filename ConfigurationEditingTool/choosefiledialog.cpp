#include "choosefiledialog.h"
#include "ui_choosefiledialog.h"

#include "QFileDialog"

ChooseFileDialog::ChooseFileDialog(QWidget *parent) :
   QDialog(parent),
   ui(new Ui::ChooseFileDialog)
{
   ui->setupUi(this);
}

ChooseFileDialog::~ChooseFileDialog()
{
   delete ui;
}

QString ChooseFileDialog::GetFilename() const
{
   return ui->pathEdit->text();
}

void ChooseFileDialog::on_buttonBox_accepted()
{
    accept();
}

void ChooseFileDialog::on_buttonBox_rejected()
{
    reject();
}

void ChooseFileDialog::on_chooseFileButton_clicked()
{
   ui->pathEdit->setText(QFileDialog::getOpenFileName(this, "Choose File to Attach"));
}
