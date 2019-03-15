#include "tasktoolrundialog.h"
#include "ui_tasktoolrundialog.h"

#include <QDir>
#include <QLabel>
#include <QMessageBox>

#include "filetools.h"
#include "pathtools.h"
#include "tools.h"

#ifdef _MSC_VER
   #include <direct.h>
#else
   #include <unistd.h>
   #define _chdir chdir
#endif

namespace
{
   QString GetErrnoMessage()
   {
      const int bufferSize = 256;
      char buffer[bufferSize];
#ifdef _MSC_VER
      strerror_s(buffer, bufferSize, errno);
#else
      strerror_r(errno, buffer, bufferSize);
#endif
      return QString(buffer);
   }

   QString GetDetailedError()
   {
      switch (errno)
      {
         case ENOENT:
            return QString("Unable to locate the directory");
         case EINVAL:
            return QString("Invalid buffer");
         default:
            return GetErrnoMessage();
      }
   }
}

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

void TaskToolRunDialog::SetReportFolder(const QString& value)
{
   reportFolder = value;
}

void TaskToolRunDialog::SetReportType(const std::string& value)
{
   reportType = value;
}

void TaskToolRunDialog::on_runButton_clicked()
{
   CleanPreviousReport();

   ui->outputTextEdit->setPlainText("");
   QString outputText;
   const std::string currentDirectory = PathTools::GetCurrentFullPath();
   int result = _chdir(runPath.toLocal8Bit());
   if (result == 0)
   {
      std::string commandOutput;
      result = Tools::RunExternalCommandToBuffer(
                      CreateTaskToolCommand(),
                      commandOutput, true
                      );

      _chdir(currentDirectory.c_str());

      outputText = (result > -1) ? commandOutput.c_str() : CreateExecutionErrorMessage(result, commandOutput);
   }
   else
   {
      outputText = CreateChdirErrorMessage();
      ui->taskWidget->setCurrentIndex(1);
   }

   ui->outputTextEdit->setPlainText(outputText);   
   SetupReportDisplay();
   SetupReportFilesDisplay();
}

std::string TaskToolRunDialog::CreateTaskToolCommand() const
{
   return std::string("\"") + taskToolExecutable.toStdString() + "\" --conffile " +
         configurationFile.toStdString();
}

QString TaskToolRunDialog::CreateChdirErrorMessage() const
{
   QString errorMessage = "Failed to change directory : " + GetDetailedError() + "\n";
   errorMessage += "  Target directory : " + runPath + "\n";
   errorMessage += "  Current directory : " + QString(PathTools::GetCurrentFullPath().c_str()) + "\n";
   return errorMessage;
}

QString TaskToolRunDialog::CreateExecutionErrorMessage(const int returnValue,
                                                       const std::string& output) const
{
   QString errorMessage = "Error running Task tool command.\n";
   errorMessage += "\tError code : " + QString::number(returnValue) + "\n";
   errorMessage += "\tOutput : " + QString(output.c_str()) + "\n";
   errorMessage += "\tRun Path : " + runPath + "\n";
   errorMessage += "\tExecutable : " + taskToolExecutable + "\n";
   errorMessage += "\tConfiguration file : " + configurationFile + "\n";
   return errorMessage;
}

void TaskToolRunDialog::SetupReportDisplay()
{
   const QString reportFileFullName = reportFolder + reportFile;
   const std::string reportContent = FileTools::GetTextFileContent(
                                        reportFileFullName.toStdString());
   const bool isHtml = (reportType == "html");
   if (isHtml)
   {
      ui->textBrowser->setHtml(reportContent.c_str());
      //ui->textBrowser->setStyleSheet();
   }
   else
      ui->textBrowser->setPlainText(reportContent.c_str());
}

void TaskToolRunDialog::SetupReportFilesDisplay()
{
   ClearFileBox();
   QStringList reportFiles = FindReportFiles();
   for (const auto& it : reportFiles)
      AddFileLink(it);
}

void TaskToolRunDialog::ClearFileBox()
{
   QLayoutItem* item;
   while ((item = ui->filesBox->layout()->takeAt(0)) != 0)
   {
      if (item->widget())
         item->widget()->setParent(nullptr);
      delete item;
   }
}

QStringList TaskToolRunDialog::FindReportFiles() const
{
   QDir directory(reportFolder);
   QStringList allFiles = directory.entryList(QDir::Files);
   allFiles.removeAll(reportFile);
   return allFiles;
}

void TaskToolRunDialog::AddFileLink(const QString& file)
{
   auto fileLabel = new QLabel(this);
   fileLabel->setOpenExternalLinks(true);
   fileLabel->setText(BuildTextLabel(file));
   ui->filesBox->layout()->addWidget(fileLabel);
}

QString TaskToolRunDialog::BuildTextLabel(const QString& file) const
{
   const QString fullFileName = "file:///" + reportFolder + file;
   QString label = "<a href=\"" + fullFileName;
   label += QString("\">") + file + "</a>";
   return label;
}

void TaskToolRunDialog::CleanPreviousReport()
{
   const std::string stdReportFolder = reportFolder.toStdString();
   if (FileTools::FolderExists(stdReportFolder))
   {
      const bool ok = FileTools::RemoveFolder(stdReportFolder, true);
      if (ok == false)
         QMessageBox::warning(this, "Error", "Previous Report data could not be cleaned\n"
                              "Please make sure Report data is cleaned before each Run,\n"
                              "otherwise previous data will be present in current report.");
   }
}