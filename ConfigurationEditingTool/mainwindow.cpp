#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <string>
#include <vector>

#include <QFileDialog>
#include <QMenu>

#include "configurationcheckdialog.h"
#include "wakejobdelegate.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
   QMainWindow(parent),
   ui(new Ui::MainWindow)
{
   ui->setupUi(this);

   jobListModel.setColumnCount(1);
   ui->jobListView->setModel(&jobListModel);

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
   jobListModel.clear();

   const QStringList jobList = model.GetJobList();
   for (auto&& it : jobList)
      jobListModel.appendRow(new QStandardItem(it));
}

void MainWindow::InsertNewJob(const QString& name)
{   
   const int currentIndex = ui->jobListView->currentIndex().row();
   jobListModel.insertRow(currentIndex+1, new QStandardItem(name));
   ui->jobListView->setItemDelegateForRow(currentIndex+1, new WakeJobDelegate());
}

void MainWindow::MoveItem(const int currentIndex, const int newIndex)
{
   QStandardItem* currentItem = jobListModel.takeItem(currentIndex);
   QStandardItem* newItem = jobListModel.takeItem(newIndex);
   jobListModel.setItem(currentIndex, newItem);
   jobListModel.setItem(newIndex, currentItem);

   MoveDelegates(currentIndex, newIndex);

   ui->jobListView->setCurrentIndex(currentItem->index());
}

void MainWindow::MoveDelegates(const int currentIndex, const int newIndex)
{
   QAbstractItemDelegate* currentDelegate = ui->jobListView->itemDelegateForRow(currentIndex);
   QAbstractItemDelegate* newDelegate = ui->jobListView->itemDelegateForRow(newIndex);
   ui->jobListView->setItemDelegateForRow(currentIndex, newDelegate);
   ui->jobListView->setItemDelegateForRow(newIndex, currentDelegate);
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
   jobListModel.removeRow(currentIndex);
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
   InsertNewJob("Wake");
}

void MainWindow::on_actionShutdown_triggered()
{
   InsertNewJob("Shutdown");
}

void MainWindow::on_actionBackup_triggered()
{
   InsertNewJob("Backup");
}

void MainWindow::on_actionCustom_command_triggered()
{
   InsertNewJob("Custom command");
}
