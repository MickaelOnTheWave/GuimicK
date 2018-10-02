#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <string>
#include <vector>

#include <QFileDialog>
#include <QMenu>

#include "aboutdialog.h"
#include "clientjobsconfiguration.h"
#include "configurationcheckdialog.h"
#include "serverconfiguration.h"

#include "jobdelegate.h"
#include "jobeditdialogfactory.h"
#include "selectbackupfolderdialog.h"

#include "abstractbackupjobdisplay.h"
#include "abstractjobdisplay.h"
#include "consolejobdisplay.h"
#include "diskspacejobdisplay.h"
#include "wakejobdisplay.h"

#include "gitbackupjob.h"
#include "gitfsbackupjob.h"
#include "linuxfreespacecheckjob.h"
#include "linuxshutdownjob.h"
#include "rawcopyfsbackupjob.h"
#include "rsnapshotsmartbackupjob.h"
#include "rsynccopyfsbackupjob.h"
#include "userconsolejob.h"
#include "libwakejob.h"
#include "zipandcopyfsbackupjob.h"

using namespace std;

const string version = "0.6";

MainWindow::MainWindow(const bool _isServerMode,
                       QWidget *parent) :
   QMainWindow(parent),
   ui(new Ui::MainWindow),
   isServerMode(_isServerMode)
{
   ui->setupUi(this);
   ui->jobListView->setModel(&jobListModel);
   ui->jobListView->setItemDelegate(new JobDelegate(new AbstractJobDisplay()));
   ui->jobListView->setResizeMode(QListView::Adjust);
   ui->checkBackupsButton->setVisible(false);

   model.SetConfigurationManager(CreateConfigurationManager());
   model.SetDefaultServerOptions();

   OpenStandardFile();
}

MainWindow::~MainWindow()
{
   delete ui;
}

void MainWindow::on_actionNew_triggered()
{
   model.ClearJobs();
   UpdateJobListWidget();
}

void MainWindow::on_actionOpen_triggered()
{
   QString filename = QFileDialog::getOpenFileName(
                         this, "Choose a configuration file to open",
                         "/home", "Configuration files (*)");

   if (filename != "")
      OpenFile(filename, true);
}

void MainWindow::on_actionSave_triggered()
{
   QString filename = QFileDialog::getSaveFileName(
                         this, "Choose a file to save",
                         "/home", "Configuration files (*)");

   if (filename != "")
   {
      model.SetJobs(jobListModel.GetJobs());
      model.SaveConfiguration(filename.toStdString());
   }
}

void MainWindow::on_actionClose_triggered()
{
   close();
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
   const int maxIndex = jobListModel.rowCount();
   const int currentIndex = ui->jobListView->currentIndex().row();
   const int insertIndex = (currentIndex+1 > maxIndex) ? maxIndex : currentIndex+1;

   UpdateRowDelegatesFromBottom(insertIndex);
   jobListModel.Insert(insertIndex, job);
   ui->jobListView->setItemDelegateForRow(insertIndex, new JobDelegate(CreateDisplay(job)));
   ui->jobListView->setCurrentIndex(jobListModel.index(insertIndex));

   ForceJobListViewUpdate();
}

void MainWindow::MoveItem(const int currentIndex, const int newIndex)
{
   if (IsOutOfBounds(currentIndex) || IsOutOfBounds(newIndex))
      return;

   jobListModel.MoveJob(currentIndex, newIndex);
   MoveDelegates(currentIndex, newIndex);
   ui->jobListView->setCurrentIndex(jobListModel.index(newIndex));
   ForceJobListViewUpdate();
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
   else if (dynamic_cast<LinuxFreeSpaceCheckJob*>(job))
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
   }
}

void MainWindow::OpenStandardFile()
{
   const QString standardFile = QDir::homePath() + "/.taskmanager";
   QFileInfo checkFile(standardFile);
   if (checkFile.exists() && checkFile.isFile())
      OpenFile(standardFile, false);
}

void MainWindow::OpenFile(const QString& filename, const bool showStatusIfOk)
{
   vector<string> errors;
   const bool isUsable = model.LoadConfiguration(filename.toStdString(), errors);
   const bool isStatusFullOk = (isUsable && errors.empty());

   if (showStatusIfOk || !isStatusFullOk)
      ConfigurationCheckDialog::Show(!isUsable, errors);
   if (isUsable)
      UpdateJobListWidget();
}

void MainWindow::on_actionWake_triggered()
{
   InsertNewJob(new LibWakeJob());
}

void MainWindow::on_actionShutdown_triggered()
{
   InsertNewJob(new LinuxShutdownJob());
}

void MainWindow::on_actionCustom_command_triggered()
{
   InsertNewJob(new UserConsoleJob());
}

void MainWindow::on_jobListView_doubleClicked(const QModelIndex &index)
{
   AbstractJob* job = jobListModel.GetJob(index);
   AbstractEditJobDialog* editDialog = JobEditDialogFactory::Create(job);

   if (editDialog)
   {
      editDialog->exec();
      delete editDialog;
   }
}

void MainWindow::on_actionRaw_Copy_triggered()
{
    InsertNewJob(new RawCopyFsBackupJob());
}

void MainWindow::on_actionRsync_Copy_triggered()
{
    InsertNewJob(new RsyncCopyFsBackupJob());
}

void MainWindow::on_actionGit_Filesystems_triggered()
{
    InsertNewJob(new GitFsBackupJob());
}

void MainWindow::on_actionGit_Repositories_triggered()
{
    InsertNewJob(new GitBackupJob());
}

void MainWindow::on_actionRsnapshot_triggered()
{
    InsertNewJob(new RsnapshotSmartBackupJob());
}

void MainWindow::on_actionZip_Copy_triggered()
{
    InsertNewJob(new ZipAndCopyFsBackupJob());
}

void MainWindow::on_actionDisk_space_check_triggered()
{
    InsertNewJob(new LinuxFreeSpaceCheckJob());
}

void MainWindow::on_actionAbout_triggered()
{
   AboutDialog dialog(version, isServerMode);
   dialog.exec();
}

void MainWindow::on_actionCustom_command_client_triggered()
{
   InsertNewJob(new SshConsoleJob(new UserConsoleJob));
}

void MainWindow::on_jobListView_clicked(const QModelIndex &index)
{
   AbstractJob* job = jobListModel.GetJob(index);
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
      vector<pair<string,string> > folders;
      job->GetFolderList(folders);
      selectedBackupFolder = folders.front().first.c_str();
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
      AbstractBackupJob* job, const QString& backupFolder, const int timeIndex) const
{
   QString folderName = QFileDialog::getExistingDirectory(
                           nullptr, "Choose a folder to restore Backup to",
                           "/home");

   // TODO : fix that
   const BackupRestoreParameters parameters = {folderName.toStdString(), 0, 0};
   const BackupRestoreTarget target = {"192.168.1.256", "user", "userPassword"};
   if (folderName != "")
      job->RestoreBackupFromClient(parameters, target);
}

TaskManagerConfiguration* MainWindow::CreateConfigurationManager()
{
   if (isServerMode)
      return new ServerConfiguration();
   else
      return new ClientJobsConfiguration();
}
