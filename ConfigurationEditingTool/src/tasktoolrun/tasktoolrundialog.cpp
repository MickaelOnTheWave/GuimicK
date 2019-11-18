#include "tasktoolrundialog.h"
#include "ui_tasktoolrundialog.h"

#include <QDir>
#include <QLabel>
#include <QMessageBox>
#include <QVBoxLayout>

#include "errormessagecreator.h"
#include "filetools.h"
#include "tasktoolrun/linuxtasktoolrunner.h"
#include "ostools.h"
#include "pathtools.h"
#include "tools.h"

#ifdef _MSC_VER
   #include <direct.h>
   #include "tasktoolrun/windows/windowstasktoolrunner.h"
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

   const int taskToolEmptyPage = 0;
   const int taskToolWaitPage = 1;
   const int taskToolReportPage = 2;
   const int taskToolOutputPage = 3;
}

TaskToolRunDialog::TaskToolRunDialog(QWidget *parent, const bool _needsAdminRights) :
   QDialog(parent),
   ui(new Ui::TaskToolRunDialog),
   needsAdminRights(_needsAdminRights)
{
   ui->setupUi(this);
   setWindowFlag(Qt::WindowMaximizeButtonHint);
   InitializeThreadedTaskToolRun();
   if (needsAdminRights)
      AddAdminRightsWarning();
}

TaskToolRunDialog::~TaskToolRunDialog()
{
   delete ui;
   taskToolThread.quit();
   taskToolThread.wait();
   delete taskToolRunner;
}

void TaskToolRunDialog::SetConfigurationData(const TaskToolConfigurationData& data)
{
   configData = data;
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
   ui->taskWidget->setCurrentIndex(taskToolEmptyPage);
   CleanPreviousReport();

   ui->outputTextEdit->setPlainText("");
   QString outputText;
   currentDirectory = PathTools::GetCurrentFullPath();
   if (PathTools::ChangeCurrentDir(configData.runPath.toStdWString()))
   {
      SetUiWaitState();
      StartTaskTool();
   }
   else
      UpdateTaskToolUiWithResults(false, CreateChdirErrorMessage());
}

void TaskToolRunDialog::OnFinishedRunningTaskTool()
{
   PathTools::ChangeCurrentDir(currentDirectory);
   UpdateTaskToolUiWithRunResult();
   UnfreezeUi();
}

std::wstring TaskToolRunDialog::CreateTaskToolCommand() const
{
#ifdef _WIN32
   return CreateWindowsTaskToolCommand();
#else
   return CreateLinuxTaskToolCommand();
#endif
}

std::wstring TaskToolRunDialog::CreateWindowsTaskToolCommand() const
{
   const std::wstring stringToken = L"\"";
   std::wstring command = stringToken;
   command += PathTools::ToWindowsPath(configData.executable.toStdWString()) + stringToken;
   command += L" --conffile ";
   command += PathTools::ToWindowsPath(configData.configurationFile.toStdWString());
   return command;
}

std::wstring TaskToolRunDialog::CreateLinuxTaskToolCommand() const
{
   std::wstring command = configData.executable.toStdWString();
   command += L" --conffile ";
   command += configData.configurationFile.toStdWString();
   return command;
}

QString TaskToolRunDialog::CreateChdirErrorMessage() const
{
   QString errorMessage = "Failed to change directory : " + GetDetailedError() + "\n";
   errorMessage += "  Target directory : " + configData.runPath + "\n";
   errorMessage += "  Current directory : " + QString::fromStdWString(PathTools::GetCurrentFullPath()) + "\n";
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
   while ((item = ui->filesBox->layout()->takeAt(0)) != nullptr)
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

void TaskToolRunDialog::AddAdminRightsWarning()
{
   warningWidget = new AdminRightsWarning(this);
   auto mainLayout = static_cast<QVBoxLayout*>(layout());
   mainLayout->insertWidget(0, warningWidget);
}

void TaskToolRunDialog::InitializeThreadedTaskToolRun()
{
   taskToolRunner = CreateRunner();
   taskToolRunner->moveToThread(&taskToolThread);
   connect(taskToolRunner, &AbstractTaskToolRunner::finished, this, &TaskToolRunDialog::OnFinishedRunningTaskTool);
}

void TaskToolRunDialog::StartTaskTool()
{
   taskToolRunner->SetCommand(CreateTaskToolCommand());
   taskToolThread.start();
   taskToolRunner->Run();
}

void TaskToolRunDialog::SetUiWaitState()
{
   setEnabled(false);
   QApplication::setOverrideCursor(Qt::WaitCursor);
   ui->taskWidget->setCurrentIndex(taskToolWaitPage);
   ui->taskWidget->repaint();
}

void TaskToolRunDialog::UnfreezeUi()
{
   setEnabled(true);
   QApplication::restoreOverrideCursor();
}

void TaskToolRunDialog::UpdateTaskToolUiWithRunResult()
{
   ErrorMessageCreator* messageCreator = taskToolRunner->CreateMessageCreator();
   const TaskToolRunData runData = taskToolRunner->GetRunData();
   const QString displayOutput = messageCreator->CreateMessage(configData, runData);
   UpdateTaskToolUiWithResults(runData.isOk, displayOutput);
}

void TaskToolRunDialog::UpdateTaskToolUiWithResults(const bool success, const QString& output)
{
   ui->outputTextEdit->setPlainText(output);
   SetupReportDisplay();
   SetupReportFilesDisplay();
   ui->taskWidget->setCurrentIndex(success ? taskToolReportPage : taskToolOutputPage);
}

AbstractTaskToolRunner* TaskToolRunDialog::CreateRunner()
{
#ifdef _WIN32
   if (needsAdminRights)
      return new WindowsTaskToolRunner();
   else
      return new LinuxTaskToolRunner();
#else
   return new LinuxTaskToolRunner();
#endif
}
