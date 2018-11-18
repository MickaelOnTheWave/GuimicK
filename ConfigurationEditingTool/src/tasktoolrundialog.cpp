#include "tasktoolrundialog.h"
#include "ui_tasktoolrundialog.h"

#include <QSettings>

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

void TaskToolRunDialog::on_runButton_clicked()
{
   QSettings settings;
   const QString runFolder = settings.value("tempFolder").toString();
   const QString toolPath = settings.value("taskTool").toString();
}
