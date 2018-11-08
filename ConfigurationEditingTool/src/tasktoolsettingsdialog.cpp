#include "tasktoolsettingsdialog.h"
#include "ui_tasktoolsettingsdialog.h"

#include <QFileDialog>

TaskToolSettingsDialog::TaskToolSettingsDialog(QWidget *parent) :
   QDialog(parent),
   ui(new Ui::TaskToolSettingsDialog)
{
   ui->setupUi(this);
}

TaskToolSettingsDialog::~TaskToolSettingsDialog()
{
   delete ui;
}

QString TaskToolSettingsDialog::GetTaskToolExecutablePath() const
{
   return ui->taskToolEdit->text();
}

void TaskToolSettingsDialog::SetTaskToolExecutable(const QString& value)
{
   ui->taskToolEdit->setText(value);
}

QString TaskToolSettingsDialog::GetConfigurationTempPath() const
{
   return ui->tempFolderEdit->text();
}

void TaskToolSettingsDialog::SetConfigurationTempPath(const QString& value)
{
   ui->tempFolderEdit->setText(value);
}

void TaskToolSettingsDialog::on_tempFolderButton_clicked()
{
   QString foldername = QFileDialog::getExistingDirectory(
                         this, "Choose a folder to use as temporary",
                         GetConfigurationTempPath(),
                         QFileDialog::ShowDirsOnly);
   if (foldername != "")
      ui->tempFolderEdit->setText(foldername);
}

void TaskToolSettingsDialog::on_taskToolButton_clicked()
{
#ifdef _WIN32
   const QString fileFilter = "Executable file (*.exe)";
#else
   const QString fileFilter = "Executable file (*)";
#endif

   const QString filename = QFileDialog::getOpenFileName(
                         this, "Select Task tool Executable",
                         GetTaskToolExecutablePath(),
                         fileFilter
                         );
   if (filename != "")
      ui->taskToolEdit->setText(filename);
}
