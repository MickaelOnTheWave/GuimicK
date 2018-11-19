#include "tasktoolrundialog.h"
#include "ui_tasktoolrundialog.h"

#include <unistd.h>
#include "filetools.h"
#include "tools.h"

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

void TaskToolRunDialog::SetRunPath(const QString& value)
{
   runPath = value;
}

void TaskToolRunDialog::SetConfigurationFile(const QString& value)
{
   configurationFile = value;
}

void TaskToolRunDialog::SetToolExecutable(const QString& value)
{
   taskToolExecutable = value;
}

void TaskToolRunDialog::SetReportFile(const QString& value)
{
   reportFile = value;
}

void TaskToolRunDialog::on_runButton_clicked()
{
   ui->outputTextEdit->setPlainText("");
   QString outputText;
   const std::string currentDirectory = FileTools::GetCurrentFullPath();
   int result = chdir(runPath.toStdString().c_str());
   if (result == 0)
   {
      std::string commandOutput;
      result = Tools::RunExternalCommandToBuffer(
                      CreateTaskToolCommand(),
                      commandOutput, true
                      );

      chdir(currentDirectory.c_str());

      outputText = (result > -1) ? commandOutput.c_str() : CreateExecutionErrorMessage(result);
   }
   else
      outputText = CreateChdirErrorMessage();

   ui->outputTextEdit->setPlainText(outputText);   
   SetupReportDisplay();
}

std::string TaskToolRunDialog::CreateTaskToolCommand() const
{
   return taskToolExecutable.toStdString() + " --conffile " +
         configurationFile.toStdString();
}

QString TaskToolRunDialog::CreateChdirErrorMessage() const
{
   QString errorMessage = "Error : failed to change directory.\n";
   errorMessage += "\tTarget directory : " + runPath + "\n";
   errorMessage += "\tCurrent directory : " + QString(FileTools::GetCurrentFullPath().c_str()) + "\n";
   return errorMessage;
}

QString TaskToolRunDialog::CreateExecutionErrorMessage(const int returnValue) const
{
   QString errorMessage = "Error running Task tool command.\n";
   errorMessage += "\tError code : " + QString::number(returnValue) + "\n";
   errorMessage += "\tRun Path : " + runPath + "\n";
   errorMessage += "\tExecutable : " + taskToolExecutable + "\n";
   errorMessage += "\tConfiguration file : " + configurationFile + "\n";
   return errorMessage;
}

void TaskToolRunDialog::SetupReportDisplay()
{
   const std::string reportContent = FileTools::GetTextFileContent(reportFile.toStdString());
   const bool isHtml = true;
   if (isHtml)
   {
      ui->textBrowser->setHtml(reportContent.c_str());
      //ui->textBrowser->setStyleSheet();
   }
   else
      ui->textBrowser->setPlainText(reportContent.c_str());
}
