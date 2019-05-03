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

void TaskToolRunDialog::SetReportType(const std::wstring& value)
{
   reportType = value;
}

void TaskToolRunDialog::on_runButton_clicked()
{
   CleanPreviousReport();

   ui->outputTextEdit->setPlainText("");
   QString outputText;
   const std::wstring currentDirectory = PathTools::GetCurrentFullPath();
   if (PathTools::ChangeCurrentDir(runPath.toStdWString()))
   {
      std::wstring commandOutput;
      const int result = Tools::RunExternalCommandToBuffer(
                      CreateTaskToolCommand(),
                      commandOutput, true
                      );

      PathTools::ChangeCurrentDir(currentDirectory);

      outputText = (result > -1) ? QString::fromStdWString(commandOutput)
                                 : CreateExecutionErrorMessage(result, commandOutput);
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

std::wstring TaskToolRunDialog::CreateTaskToolCommand() const
{
   std::wstring command = std::wstring(L"\"");
   command += PathTools::ToWindowsPath(taskToolExecutable.toStdWString()) + L"\" --conffile \"";
   command += PathTools::ToWindowsPath(configurationFile.toStdWString()) + L"\"";
   return command;
}

QString TaskToolRunDialog::CreateChdirErrorMessage() const
{
   QString errorMessage = "Failed to change directory : " + GetDetailedError() + "\n";
   errorMessage += "  Target directory : " + runPath + "\n";
   errorMessage += "  Current directory : " + QString::fromStdWString(PathTools::GetCurrentFullPath()) + "\n";
   return errorMessage;
}

QString TaskToolRunDialog::CreateExecutionErrorMessage(const int returnValue,
                                                       const std::wstring& output) const
{
   QString errorMessage = "Error running Task tool command.\n";
   errorMessage += "\tError code : " + QString::number(returnValue) + "\n";
   errorMessage += "\tOutput : " + QString::fromStdWString(output) + "\n";
   errorMessage += "\tRun Path : " + runPath + "\n";
   errorMessage += "\tExecutable : " + taskToolExecutable + "\n";
   errorMessage += "\tConfiguration file : " + configurationFile + "\n";
   return errorMessage;
}

void TaskToolRunDialog::SetupReportDisplay()
{
   const QString reportFileFullName = reportFolder + reportFile;
   const std::wstring reportContent = FileTools::GetTextFileContent(
                                        reportFileFullName.toStdWString());
   const QString qReportContent = QString::fromStdWString(reportContent);
   const bool isHtml = (reportType == L"html");
   if (isHtml)
   {
      ui->textBrowser->setHtml(qReportContent);
      //ui->textBrowser->setStyleSheet();
   }
   else
      ui->textBrowser->setPlainText(qReportContent);
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
   const std::wstring stdReportFolder = reportFolder.toStdWString();
   if (FileTools::FolderExists(stdReportFolder))
   {
      const bool ok = FileTools::RemoveFolder(stdReportFolder, true);
      if (ok == false)
         QMessageBox::warning(this, "Error", "Previous Report data could not be cleaned\n"
                              "Please make sure Report data is cleaned before each Run,\n"
                              "otherwise previous data will be present in current report.");
   }
}
