#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <string>
#include <vector>

#include <QFileDialog>
#include <QMenu>

#include "jobdelegate.h"

#include "abstractjobdisplay.h"
#include "wakejobdisplay.h"

#include "configurationcheckdialog.h"
#include "editconsolejobdialog.h"
#include "editshutdownjobdialog.h"
#include "editwakejobdialog.h"

#include "linuxshutdownjob.h"
#include "rawcopyfsbackupjob.h"
#include "userconsolejob.h"
#include "wakejob.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
   QMainWindow(parent),
   ui(new Ui::MainWindow)
{
   ui->setupUi(this);
   ui->jobListView->setModel(&jobListModel);
   ui->jobListView->setItemDelegate(new JobDelegate(new AbstractJobDisplay()));
   ui->jobListView->setResizeMode(QListView::Adjust);

   OpenStandardFile();
}

MainWindow::~MainWindow()
{
   delete ui;
}

void MainWindow::on_actionNew_triggered()
{

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
      model.SaveConfiguration(filename.toStdString());
}

void MainWindow::on_actionClose_triggered()
{
   close();
}

void MainWindow::UpdateJobListWidget()
{
   jobListModel.Clear();
   jobListModel.AddJobs(model.GetJobList());
}

void MainWindow::InsertNewJob(AbstractJob* job)
{   
   const int currentIndex = ui->jobListView->currentIndex().row();
   UpdateRowDelegatesFromBottom(currentIndex+1);
   jobListModel.Insert(currentIndex+1, job);
   ui->jobListView->setItemDelegateForRow(currentIndex+1, new JobDelegate(CreateDisplay(job)));
   ui->jobListView->setCurrentIndex(jobListModel.index(currentIndex+1));

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

AbstractEditJobDialog* MainWindow::CreateEditDialog(AbstractJob* job) const
{
   if (dynamic_cast<WakeJob*>(job))
      return new EditWakeJobDialog(job);
   else if (dynamic_cast<LinuxShutdownJob*>(job))
      return new EditShutdownJobDialog(job);
   else if (dynamic_cast<AbstractConsoleJob*>(job))
      return new EditConsoleJobDialog(job);
   else
      return nullptr;
}

AbstractDisplay* MainWindow::CreateDisplay(AbstractJob* job) const
{
   if (dynamic_cast<WakeJob*>(job))
      return new WakeJobDisplay();
   else
      return new AbstractJobDisplay();
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

void MainWindow::on_addButton_clicked()
{
/*   QMenu addMenu(this);
   addMenu.addAction("Wake");
   addMenu.addAction("Shutdown");
   addMenu.popup(QPoint(10,10));*/
}

void MainWindow::on_deleteButton_clicked()
{
   const int currentIndex = ui->jobListView->currentIndex().row();
   UpdateRowDelegatesFromTop(currentIndex);
   jobListModel.removeRow(currentIndex);
   ForceJobListViewUpdate();
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
   InsertNewJob(new WakeJob());
}

void MainWindow::on_actionShutdown_triggered()
{
   InsertNewJob(new LinuxShutdownJob());
}

void MainWindow::on_actionBackup_triggered()
{
   InsertNewJob(new RawCopyFsBackupJob());
}

void MainWindow::on_actionCustom_command_triggered()
{
   InsertNewJob(new UserConsoleJob());
}

void MainWindow::on_jobListView_doubleClicked(const QModelIndex &index)
{
   AbstractJob* job = jobListModel.GetJob(index);
   AbstractEditJobDialog* editDialog = CreateEditDialog(job);

   if (editDialog)
   {
      editDialog->exec();
      delete editDialog;
   }
}
