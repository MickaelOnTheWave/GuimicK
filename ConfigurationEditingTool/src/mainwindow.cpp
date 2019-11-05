#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <string>
#include <vector>

#include <QCloseEvent>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>
#include <QSettings>
#include <QStandardPaths>

#include "aboutdialog.h"
#include "clientjobsconfiguration.h"
#include "configurationcheckdialog.h"
#include "configurationtype.h"
#include "configurationtypedialog.h"
#include "editorversion.h"
#include "filetools.h"
#include "jobdelegate.h"
#include "jobeditdialogfactory.h"
#include "ostools.h"
#include "pathtools.h"
#include "scheduler/schedulerdialog.h"
#include "selectbackupfolderdialog.h"
#include "serverconfiguration.h"
#include "settingsdialog.h"
#include "tasktoolrundialog.h"
#include "tasktoolsettingsdialog.h"

#include "abstractbackupjobdisplay.h"
#include "abstractjobdisplay.h"
#include "consolejobdisplay.h"
#include "diskspacejobdisplay.h"
#include "wakejob/wakejobdisplay.h"

#include "gitbackupjob.h"
#include "gitfsbackupjob.h"
#include "linuxfreespacecheckjob.h"
#include "linuxshutdownjob.h"
#include "oscopyfsbackupjob.h"
#include "rsnapshotsmartbackupjob.h"
#include "rsynccopyfsbackupjob.h"
#include "sevenzipbackupjob.h"
#include "tarbackupjob.h"
#include "userconsolejob.h"
#include "libwakejob.h"
#include "windowscheckdiskjob.h"

#ifdef WIN32
   #include <Windows.h>
   #undef GetJob
#endif


using namespace std;

namespace
{
   const QString iconBackupCopy = ":/icons/folderBackup";
   const QString iconBackup7zip = ":/icons/7zip";
   const QString iconConsole = ":/icons/console_50";
   const QString iconHd = ":/icons/hd_50";

   QString GetDefaultTaskToolExecutable()
   {
#ifdef _MSC_VER
      const std::wstring path = PathTools::GetCurrentExecutablePath();
      const std::wstring taskToolExe = PathTools::GetPathOnly(path) + L"\\TaskTool.exe";
      return QString::fromStdWString(taskToolExe);
#else
      return QString("/usr/local/bin/taskmanagerTool");
#endif

   }

   QString GetTaskToolExecutable()
   {
      QSettings settings;
      
      QVariant keyValue = settings.value("taskTool", GetDefaultTaskToolExecutable());
      if (keyValue.isValid())
         return keyValue.toString();
      else
         return QString("");
   }

   wstring CreateReportFile(const wstring& type)
   {
      wstring filename = L"report.";
      if (type == L"html")
         filename += L"html";
      else if (type == L"text")
         filename += L"txt";
      else
         filename += L"log";
      return filename;
   }

   QString GetDefaultWriteableTempFolder()
   {
      const QStringList defaultFolders = QStandardPaths::standardLocations(QStandardPaths::TempLocation);
      return (defaultFolders.empty()) ? QString("") : defaultFolders.first();
   }

   void SetDefaultConfigurationValues(TooledConfiguration& configuration)
   {
      Agent* agent = configuration.GetAgent();
      agent->SetName(L"Task Manager Agent");
      agent->SetReportFile(L"report.html");

      const QString defaultFolder = QDir::homePath() + "/Task Manager Report/";
      agent->SetReportFolder(defaultFolder.toStdWString());

      const QString defaultCss = QDir::currentPath() + "/report.css";
      configuration.GetTmpConfiguration()->SetReportCss(defaultCss.toStdWString());
   }
}

MainWindow::MainWindow(QWidget *parent) :
   QMainWindow(parent),
   ui(new Ui::MainWindow),
   configurationType(StandaloneConfigurationType)
{
   ui->setupUi(this);
   ui->jobListView->setModel(&jobListModel);
   ui->jobListView->setItemDelegate(new JobDelegate(new AbstractJobDisplay()));
   ui->jobListView->setResizeMode(QListView::Adjust);
   ui->actionTask_Tool->setVisible(EditorVersion::HasDevelopmentFeatures());

   SetDefaultJobsButtonsState();

   MoveToScreenCenter();

   model.SetDefaultServerOptions();
   SetupAddJobMenu();
   isRunningAsAdministrator = OsTools::IsRunningAsAdministrator();
}

MainWindow::~MainWindow()
{
   delete ui;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
   const bool proceed = ManageUserActionIfModified();
   if (proceed)
      QWidget::closeEvent(event);
   else
      event->ignore();
}

void MainWindow::on_actionNew_triggered()
{
   const bool proceed = ManageUserActionIfModified();
   if (!proceed)
      return;

   TryCreatingNewFile();
}

void MainWindow::on_actionOpen_triggered()
{
   const bool proceed = ManageUserActionIfModified();
   if (!proceed)
      return;

   QString filename = QFileDialog::getOpenFileName(
                         this, "Choose a configuration file to open",
                         QDir::homePath(), "Configuration files (*)");

   if (filename != "")
      OpenFile(filename, false);
}

void MainWindow::on_actionSave_triggered()
{
   if (currentConfigurationFile != "")
      SaveFile(currentConfigurationFile);
   else
      on_actionSave_As_triggered();
}

void MainWindow::on_actionSave_As_triggered()
{
   QString filename = QFileDialog::getSaveFileName(
                         this, "Choose a file to save",
                         QDir::homePath(), "Configuration files (*)");

   if (filename != "")
   {
      const bool ok = SaveFile(filename);
      if (ok)
      {
         currentConfigurationFile = filename;
         setWindowTitle(CreateWindowTitle());
      }
   }
}

void MainWindow::on_actionQuit_triggered()
{
   close();
}

void MainWindow::on_actionGeneral_triggered()
{
   SettingsDialog dialog(model.GetTmpConfiguration(), this);
   const int result = dialog.exec();
   if (result == QDialog::Accepted)
   {
      hasConfigurationChanged = true;
      UpdateModificationStatus();
   }
}

void MainWindow::on_actionAbout_triggered()
{
   AboutDialog dialog(configurationType);
   dialog.exec();
}

void MainWindow::UpdateJobListWidget()
{
   jobListModel.Clear();

   const list<AbstractJob*> jobs = model.GetJobList();
   jobListModel.AddJobs(jobs);
   CreateJobDisplayDelegates(jobs);
   ForceJobListViewUpdate();
}

void MainWindow::InsertNewJob(AbstractJob* job)
{
   SetNewNameIfNecessary(job);
   const int maxIndex = jobListModel.rowCount();
   const int currentIndex = ui->jobListView->currentIndex().row();
   const int insertIndex = (currentIndex+1 > maxIndex) ? maxIndex : currentIndex+1;

   UpdateRowDelegatesFromBottom(insertIndex);
   jobListModel.Insert(insertIndex, job);
   ui->jobListView->setItemDelegateForRow(insertIndex, new JobDelegate(CreateDisplay(job)));
   ui->jobListView->setCurrentIndex(jobListModel.index(insertIndex));

   ForceJobListViewUpdate();
   hasConfigurationChanged = true;
   UpdateModificationStatus();
   UpdateUiOnJobSelection(insertIndex);
}

void MainWindow::MoveItem(const int currentIndex, const int newIndex)
{
   if (IsOutOfBounds(currentIndex) || IsOutOfBounds(newIndex))
      return;

   jobListModel.MoveJob(currentIndex, newIndex);
   MoveDelegates(currentIndex, newIndex);
   ui->jobListView->setCurrentIndex(jobListModel.index(newIndex));
   ForceJobListViewUpdate();
   hasConfigurationChanged = true;
   UpdateModificationStatus();
   UpdateUiOnJobSelection(newIndex);
}

void MainWindow::MoveDelegates(const int currentIndex, const int newIndex)
{
   QAbstractItemDelegate* currentDelegate = ui->jobListView->itemDelegateForRow(currentIndex);
   QAbstractItemDelegate* newDelegate = ui->jobListView->itemDelegateForRow(newIndex);
   ui->jobListView->setItemDelegateForRow(currentIndex, newDelegate);
   ui->jobListView->setItemDelegateForRow(newIndex, currentDelegate);
}

void MainWindow::ForceJobListViewUpdate()
{
   QSize size = ui->jobListView->viewport()->size();
   size.setHeight(size.height()+1);
   ui->jobListView->viewport()->resize(size);

   size.setHeight(size.height()-1);
   ui->jobListView->viewport()->resize(size);
}

bool MainWindow::IsOutOfBounds(const int index) const
{
   return (index < 0 || index >= jobListModel.rowCount());
}

void MainWindow::UpdateRowDelegatesFromTop(const int startingIndex)
{
   for (int i=startingIndex; i<jobListModel.rowCount(); ++i)
   {
      if (IsOutOfBounds(i))
         continue;

      QAbstractItemDelegate* newDelegate = ui->jobListView->itemDelegateForRow(i+1);
      ui->jobListView->setItemDelegateForRow(i, newDelegate);
   }
}

void MainWindow::UpdateRowDelegatesFromBottom(const int startingIndex)
{
   for (int i=jobListModel.rowCount()-1; i>=startingIndex; --i)
   {
      if (IsOutOfBounds(i))
         continue;

      QAbstractItemDelegate* newDelegate = ui->jobListView->itemDelegateForRow(i-1);
      ui->jobListView->setItemDelegateForRow(i, newDelegate);
   }
}

AbstractDisplay* MainWindow::CreateDisplay(AbstractJob* job) const
{
   if (dynamic_cast<AbstractWakeJob*>(job))
      return new WakeJobDisplay();
   else if (dynamic_cast<AbstractBackupJob*>(job))
      return new AbstractBackupJobDisplay();
   else if (dynamic_cast<DiskRelatedJob*>(job))
      return new DiskSpaceJobDisplay();
   else if (dynamic_cast<AbstractConsoleJob*>(job))
      return new ConsoleJobDisplay();
   else
      return new AbstractJobDisplay();
}

void MainWindow::CreateJobDisplayDelegates(const std::list<AbstractJob*>& jobs)
{
   int index = 0;
   for (const auto it : jobs)
   {
      ui->jobListView->setItemDelegateForRow(index, new JobDelegate(CreateDisplay(it)));
      ++index;
   }
}

void MainWindow::on_upButton_clicked()
{
   const int currentIndex = ui->jobListView->currentIndex().row();
   const int newIndex = (currentIndex > 0) ? currentIndex-1 : 0;

   MoveItem(currentIndex, newIndex);
}

void MainWindow::on_downButton_clicked()
{
   const int currentIndex = ui->jobListView->currentIndex().row();
   const int newIndex = (currentIndex < jobListModel.rowCount())
                        ? currentIndex+1
                        : currentIndex;

   MoveItem(currentIndex, newIndex);
}

void MainWindow::on_deleteButton_clicked()
{
   if (jobListModel.rowCount() > 0)
   {
      const int currentIndex = ui->jobListView->currentIndex().row();
      UpdateRowDelegatesFromTop(currentIndex);
      jobListModel.removeRow(currentIndex);
      ForceJobListViewUpdate();
      hasConfigurationChanged = true;
      UpdateModificationStatus();
      SetDefaultJobsButtonsState();
   }
}

void MainWindow::OpenDefaultFile()
{
   const QString standardFile = QDir::homePath() + "/.taskmanager";
   QFileInfo checkFile(standardFile);
   if (checkFile.exists() && checkFile.isFile())
      OpenFile(standardFile, false);
   else
      TryCreatingNewFile();
}

void MainWindow::CreateNewFile()
{
    model.SetConfigurationType(configurationType);
    model.ClearJobs();
    if (configurationType != ClientConfigurationType)
      SetDefaultConfigurationValues(model);
    UpdateUiOnFileChange("");
}

void MainWindow::OpenFile(const QString& filename, const bool showStatusIfOk)
{
   vector<wstring> errors;
   const bool isUsable = model.LoadConfiguration(filename.toStdWString(), errors);
   const bool isStatusFullOk = (isUsable && errors.empty());

   if (showStatusIfOk || !isStatusFullOk)
      ConfigurationCheckDialog::Show(!isUsable, errors);

   if (isUsable)
   {
      configurationType = model.GetConfigurationType();
      UpdateUiOnFileChange(filename);
   }
}

bool MainWindow::SaveFile(const QString& filename)
{
   bool ok = SaveConfigurationToFile(model, filename);
   if (ok)
   {
      hasConfigurationChanged = false;
      UpdateModificationStatus();
   }
   else
   {
      QMessageBox::warning(this, "Write Error", "Configuration file could not be saved.\n"
                           "Please check your system permissions.");
   }

   return ok;
}

bool MainWindow::SaveConfigurationToFile(TooledConfiguration& customConfig,
                                         const QString& filename)
{
   customConfig.SetJobs(jobListModel.GetJobs());
   return customConfig.SaveConfiguration(filename.toStdWString());
}

void MainWindow::QuitApplication()
{

}

void MainWindow::on_jobListView_doubleClicked(const QModelIndex &index)
{
   AbstractJob* job = jobListModel.GetJob(index);
   AbstractEditJobDialog* editDialog = JobEditDialogFactory::Create(job);
   if (editDialog)
   {
      editDialog->SetupFor(configurationType);
      const int result = editDialog->exec();
      if (result == QDialog::Accepted)
      {
         hasConfigurationChanged = true;
         UpdateModificationStatus();
      }
      delete editDialog;
   }
}

void MainWindow::on_jobListView_clicked(const QModelIndex& index)
{
   UpdateUiOnJobSelection(index.row());
   //AbstractJob* job = jobListModel.GetJob(index);
   //const bool isBackupJob = (dynamic_cast<AbstractBackupJob*>(job) != nullptr);
   const bool isBackupJob = false; // Pending - see task [056].
   ui->checkBackupsButton->setVisible(isBackupJob);   
}

void MainWindow::on_checkBackupsButton_clicked()
{
   AbstractJob* baseJob = jobListModel.GetJob(ui->jobListView->currentIndex());
   auto backupJob = dynamic_cast<AbstractBackupJob*>(baseJob);
   if (!backupJob)
      return;

   const QString backupFolderToRestore = GetBackupFolder(backupJob);
   if (backupFolderToRestore == "")
      return;

   const int backupTimeIndexToRestore = GetBackupTimeIndex(
                                           backupJob, backupFolderToRestore);
   if (backupTimeIndexToRestore == -1)
      return;

   RestoreBackup(backupJob, backupFolderToRestore, backupTimeIndexToRestore);
}

void MainWindow::MoveToScreenCenter()
{
   QDesktopWidget desktop;
   const QRect screenArea = desktop.availableGeometry();
   const QRect mainWindowArea = geometry();
   const QPoint targetPosition = screenArea.center() - mainWindowArea.center();
   move(targetPosition);
}

void MainWindow::SetupAddJobMenu()
{
   if (EditorVersion::IsWindowsVersion())
      SetupWindowsAddJobMenu();
   else
      SetupLinuxAddJobMenu();
}

void MainWindow::SetupWindowsAddJobMenu()
{
   AddJobMenuEntry(ui->menuAdd_Job, iconBackupCopy, "Simple Copy Backup", new OsCopyFsBackupJob);
   AddJobMenuEntry(ui->menuAdd_Job, iconBackup7zip, "7zip Backup", new SevenZipBackupJob);
   AddJobMenuEntry(ui->menuAdd_Job, iconHd, "Check disk", new WindowsCheckdiskJob);
   AddJobMenuEntry(ui->menuAdd_Job, iconConsole, "Custom command", new UserConsoleJob);
}

void MainWindow::SetupLinuxAddJobMenu()
{
   if (configurationType == ServerConfigurationType)
   {
      AddJobMenuEntry("Wake", new LibWakeJob);
      AddJobMenuEntry("Shutdown", new LinuxShutdownJob);
   }

   QMenu* subMenuBackup = new QMenu("Backup");
   AddJobMenuEntry(subMenuBackup, iconBackupCopy, "Raw Copy", new OsCopyFsBackupJob);
   AddJobMenuEntry(subMenuBackup, "Rsync Copy", new RsyncCopyFsBackupJob);
   AddJobMenuEntry(subMenuBackup, "Tar Backup", new TarBackupJob);
   AddJobMenuEntry(subMenuBackup, "Git (filesystems)", new GitFsBackupJob);
   AddJobMenuEntry(subMenuBackup, "Git (repositories)", new GitBackupJob);
   AddJobMenuEntry(subMenuBackup, "Rsnapshot", new RsnapshotSmartBackupJob);
   ui->menuAdd_Job->addMenu(subMenuBackup);

   AddJobMenuEntry(ui->menuAdd_Job, iconHd, "Disk Space Check", new LinuxFreeSpaceCheckJob);
   if (configurationType != StandaloneConfigurationType)
   {
      AddJobMenuEntry(ui->menuAdd_Job, iconConsole,
                      "Custom command (server)", new UserConsoleJob);
      AddJobMenuEntry(ui->menuAdd_Job, iconConsole,
                      "Custom command (client)", new SshConsoleJob(new UserConsoleJob));
   }
   else
      AddJobMenuEntry(ui->menuAdd_Job, iconConsole,
                      "Custom command", new UserConsoleJob);
}

void MainWindow::AddJobMenuEntry(const QString& title, AbstractJob* job)
{
   AddJobMenuEntry(ui->menuAdd_Job, title, job);
}

void MainWindow::AddJobMenuEntry(QMenu* subMenu,
                                 const QString& title,
                                 AbstractJob* job)
{
   subMenu->addAction(title, [this, job](){InsertNewJob(job->Clone());});
}

void MainWindow::AddJobMenuEntry(QMenu* subMenu,
                                 const QString& icon,
                                 const QString& title,
                                 AbstractJob* job)
{
   subMenu->addAction(QIcon(icon), title,
                      [this, job](){InsertNewJob(job->Clone());});
}


bool MainWindow::ManageUserActionIfModified()
{
   if (hasConfigurationChanged)
   {
      const int button = QMessageBox::warning(this, tr("Unsaved changes"),
                                     tr("Your current configuration has modifications.\n"
                                        "What to you want to do ?"),
                                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                                     QMessageBox::Cancel
                                     );
      if (button == QMessageBox::Save)
         on_actionSave_triggered();

      return (button != QMessageBox::Cancel);
   }
   else
      return true;
}

void MainWindow::TryCreatingNewFile()
{
   if (EditorVersion::IsStandaloneOnly())
   {
      configurationType = StandaloneConfigurationType;
      CreateNewFile();
   }
   else
   {
      ConfigurationTypeDialog dialog;
      if (dialog.exec() == QDialog::Accepted)
      {
         configurationType = dialog.GetChosenType();
         CreateNewFile();
      }
   }
}

QString MainWindow::GetBackupFolder(AbstractBackupJob* job) const
{
   QString selectedBackupFolder("");
   if (job->GetFolderCount() > 1)
   {
      SelectBackupFolderDialog dialog(job);
      int result = dialog.exec();
      if (result == QDialog::Accepted)
         selectedBackupFolder = dialog.GetSelectedBackup();
   }
   else if (job->GetFolderCount() == 1)
   {
      vector<pair<wstring,wstring> > folders;
      job->GetFolderList(folders);
      selectedBackupFolder = QString::fromStdWString(folders.front().first);
   }

   return selectedBackupFolder;
}

int MainWindow::GetBackupTimeIndex(
      AbstractBackupJob*, const QString&) const
{
   // TODO : implement
   return 0;
}

void MainWindow::RestoreBackup(
      AbstractBackupJob* job, const QString&, const int) const
{
   QString folderName = QFileDialog::getExistingDirectory(
                           nullptr, "Choose a folder to restore Backup to",
                           "/home");

   // TODO : fix that
   const BackupRestoreParameters parameters = {folderName.toStdWString(), 0, 0};
   const BackupRestoreTarget target = {L"192.168.1.256", L"user", L"userPassword"};
   if (folderName != "")
      job->RestoreBackupFromClient(parameters, target);
}

void MainWindow::UpdateModificationStatus()
{
   ui->actionSave->setEnabled(hasConfigurationChanged);
   setWindowTitle(CreateWindowTitle());
}

QString MainWindow::CreateWindowTitle() const
{
   const QString separator = " - ";
   const QString modificationMarker = "*";
   QString newWindowTitle = "Configuration Editing Tool";
   const bool isValidFile = (currentConfigurationFile != "");
   if (hasConfigurationChanged || isValidFile)
   {
      newWindowTitle += separator;
      if (isValidFile)
         newWindowTitle += currentConfigurationFile;
      if (hasConfigurationChanged)
         newWindowTitle += modificationMarker;
   }
   return newWindowTitle;
}

void MainWindow::UpdateUiOnFileChange(const QString& newFile)
{
   UpdateJobListWidget();
   const bool isMenuVisible = (configurationType != ClientConfigurationType);
   ui->menuSettings->menuAction()->setVisible(isMenuVisible);
   ui->menuTask_Tool->menuAction()->setVisible(isMenuVisible);

   currentConfigurationFile = newFile;
   hasConfigurationChanged = false;
   UpdateModificationStatus();
   UpdateAddJobMenuEntries();
   SetDefaultJobsButtonsState();
}

void MainWindow::UpdateAddJobMenuEntries()
{
   ui->menuAdd_Job->clear();
   SetupAddJobMenu();
}

void MainWindow::OpenRunDialog(const wstring& reportFile)
{
   TaskToolRunDialog dialog(this, NeedsAdminRightsWarning());
   dialog.SetRunPath(GetTempFolder());
   dialog.SetConfigurationFile(GetTempConfigFilename());
   dialog.SetToolExecutable(GetTaskToolExecutable());
   dialog.SetReportFile(QString::fromStdWString(reportFile));
   dialog.SetReportFolder(GetTempReportFolder());

   // TODO : implement this
   dialog.SetReportType(model.GetTmpConfiguration()->GetReportType());
   //dialog.SetReportCss();
   dialog.exec();
}

QString MainWindow::GetTempFolder() const
{
   QSettings settings;
   QVariant keyValue = settings.value("tempFolder", GetDefaultWriteableTempFolder());
   if (keyValue.isValid())
      return keyValue.toString();
   else
      return QString("");
}

QString MainWindow::GetTempConfigFilename() const
{
   return GetTempFolder() + "/tempConfiguration.txt";
}

QString MainWindow::GetTempReportFolder() const
{
   return GetTempFolder() + "/TaskManagerReport/";
}

wstring MainWindow::SaveConfigurationToTempLocation()
{
   TooledConfiguration tempModel(model);
   tempModel.GetTmpConfiguration()->SetLocalShutdown(false);
   tempModel.GetTmpConfiguration()->SetReportDispatching(L"file");

   const wstring reportFile = CreateReportFile(tempModel.GetTmpConfiguration()->GetReportType());
   const wstring reportFolder = GetTempReportFolder().toStdWString();
   tempModel.GetAgent()->SetReportFile(reportFile);
   tempModel.GetAgent()->SetReportFolder(reportFolder);

   SaveConfigurationToFile(tempModel, GetTempConfigFilename());
   return reportFile;
}

bool MainWindow::NeedsAdminRightsWarning()
{
   return (!isRunningAsAdministrator && jobListModel.DoesJobListNeedAdminRights());
}

wstring MainWindow::GetDefaultReportFolder()
{
    const QString folder = QDir::homePath() + "/Task Manager Report/";
    return folder.toStdWString();
}

wstring MainWindow::GetDefaultReportCss()
{
   const QString file = QDir::currentPath() + "/report.css";
   return file.toStdWString();
}

void MainWindow::SetupDefaultFolder()
{
    wstring reportFolder = model.GetAgent()->GetReportFolder();
    if (reportFolder == L"")
    {
        reportFolder = GetDefaultReportFolder();
        model.GetAgent()->SetReportFolder(reportFolder);
    }

    if (FileTools::FolderExists(reportFolder) == false)
    {
        const bool ok = FileTools::CreateFolder(reportFolder);
        if (!ok)
        {
            QMessageBox::warning(this, "Error with ReportFolder",
                                 "Specified report folder does not exist and can't be created.\n"
                                 "Please choose a report folder without the read-only attribute.\n"
                                 "You can specify one in the \"Settings\" section.");
        }
    }
}

void MainWindow::SetupDefaultCss()
{
   wstring reportCss = model.GetTmpConfiguration()->GetReportCss();
   if (reportCss == L"")
   {
      reportCss = GetDefaultReportCss();
      model.GetTmpConfiguration()->SetReportCss(reportCss);
   }
}

void MainWindow::UpdateUiOnJobSelection(const int index)
{
   ui->upButton->setVisible(index > 0);
   ui->downButton->setVisible(index < jobListModel.rowCount()-1);
   ui->deleteButton->setVisible(true);
}

void MainWindow::SetNewNameIfNecessary(AbstractJob* job)
{
   int index = 1;
   wstring newName = job->GetName();
   while (jobListModel.IsNameAlreadyUsed(newName))
   {
      wstringstream nameStream;
      nameStream << job->GetName() << L" " << index++;
      newName = nameStream.str();
   }
   job->SetName(newName);
}

void MainWindow::on_actionRun_triggered()
{
   SetupDefaultCss();
   const wstring reportFile = SaveConfigurationToTempLocation();
   const wstring tempConfigFilename = GetTempConfigFilename().toStdWString();
   if (FileTools::FileExists(tempConfigFilename) == false)
   {
      QMessageBox::warning(this, "Error with Temp folder",
                           "Configuration file could not be created in selected temp folder.\n"
                           "Please choose a temporary folder without the read-only attribute.\n"
                           "You can specify one in the \"Settings->Task Tool\" section.");
   }
   else
   {
      SetupDefaultFolder();
      OpenRunDialog(reportFile);
   }
}

void MainWindow::on_actionTask_Tool_triggered()
{
   TaskToolSettingsDialog dialog(this);

   dialog.SetConfigurationTempPath(GetTempFolder());
   dialog.SetTaskToolExecutable(GetTaskToolExecutable());

   int result = dialog.exec();
   if (result == QDialog::Accepted)
   {
      QSettings settings;
      settings.setValue("tempFolder", dialog.GetConfigurationTempPath());
      settings.setValue("taskTool", dialog.GetTaskToolExecutablePath());
   }
}

void MainWindow::on_actionSchedule_Execution_triggered()
{
   if (currentConfigurationFile == "")
   {
      QMessageBox::StandardButton button = QMessageBox::warning(
               this, "Configuration file not saved",
               "Current configuration hasn't been saved.\n"
               "It needs to be saved to a file before it can be scheduled for execution.\n"
               "Do you want to save current configuration?",
               QMessageBox::Yes | QMessageBox::No,
               QMessageBox::Yes
               );
      if (button == QMessageBox::Yes)
         on_actionSave_As_triggered();
   }

   if (currentConfigurationFile != "")
   {
      SchedulerDialog dialog(this, isRunningAsAdministrator, NeedsAdminRightsWarning());
      dialog.SetConfigurationFile(currentConfigurationFile);
      dialog.SetTaskToolExecutable(GetTaskToolExecutable());
      dialog.exec();
   }
}

void MainWindow::SetDefaultJobsButtonsState()
{
   ui->upButton->setVisible(false);
   ui->downButton->setVisible(false);
   ui->deleteButton->setVisible(false);
   ui->checkBackupsButton->setVisible(false);
}
