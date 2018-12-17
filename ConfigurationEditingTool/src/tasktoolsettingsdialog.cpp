#include "tasktoolsettingsdialog.h"
#include "ui_tasktoolsettingsdialog.h"

#include <QFileDialog>

TaskToolSettingsDialog::TaskToolSettingsDialog(QWidget *parent) :
   QDialog(parent),
   ui(new Ui::TaskToolSettingsDialog)
{
   ui->setupUi(this);

   InitializePathWidgets();
}

TaskToolSettingsDialog::~TaskToolSettingsDialog()
{
   delete ui;
}

QString TaskToolSettingsDialog::GetTaskToolExecutablePath() const
{
   return ui->executableSelectionWidget->GetPath();
}

void TaskToolSettingsDialog::SetTaskToolExecutable(const QString& value)
{
   ui->executableSelectionWidget->SetPath(value);
}

QString TaskToolSettingsDialog::GetConfigurationTempPath() const
{
   return ui->tempFolderWidget->GetPath();
}

void TaskToolSettingsDialog::SetConfigurationTempPath(const QString& value)
{
   ui->tempFolderWidget->SetPath(value);
}

void TaskToolSettingsDialog::InitializePathWidgets()
{
   ui->tempFolderWidget->InitializeAsFolder("Temp Folder",
                                            "Choose a folder to use as temporary",
                                            GetConfigurationTempPath());

#ifdef _WIN32
   const QString fileFilter = "Executable file (*.exe)";
#else
   const QString fileFilter = "Executable file (*)";
#endif

   ui->executableSelectionWidget->InitializeAsExistingFile(
            "TaskTool Executable",
            "Select Task tool Executable",
            GetTaskToolExecutablePath(),
            fileFilter);
}
