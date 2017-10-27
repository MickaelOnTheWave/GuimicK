#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <string>
#include <vector>

#include <QFileDialog>
#include <QMenu>

#include "configurationcheckdialog.h"
#include "wakelistwidget.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
   QMainWindow(parent),
   ui(new Ui::MainWindow)
{
   ui->setupUi(this);

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
   ui->jobListWidget->clear();
   ui->jobListWidget->addItems(model.GetJobList());
}

#include <QLabel>
void MainWindow::InsertNewJob(const QString& name)
{
   const int currentIndex = ui->jobListWidget->currentRow();

   QListWidgetItem* item = new QListWidgetItem(name);
   ui->jobListWidget->insertItem(currentIndex+1, item);

//   auto itemWidget = new WakeListWidget();
//   itemWidget->Initialize(name, 3, 30);
//   auto itemWidget = new QPushButton();


   auto itemWidget = new QWidget();
   auto layout = new QHBoxLayout();
   layout->addWidget(new QLabel("Wake"));
   layout->addWidget(new QLabel("blabla"));
   itemWidget->setLayout(layout);


   ui->jobListWidget->setItemWidget(item, itemWidget);
}


void MainWindow::on_upButton_clicked()
{
   const int currentIndex = ui->jobListWidget->currentRow();
   const int newIndex = (currentIndex > 0) ? currentIndex-1 : 0;

   QListWidgetItem* item = ui->jobListWidget->takeItem(currentIndex);
   ui->jobListWidget->insertItem(newIndex, item);
   ui->jobListWidget->setCurrentRow(newIndex);
}

void MainWindow::on_downButton_clicked()
{
   const int currentIndex = ui->jobListWidget->currentRow();
   const int newIndex = (currentIndex < ui->jobListWidget->count()-1)
                        ? currentIndex+1
                        : currentIndex;

   QListWidgetItem* item = ui->jobListWidget->takeItem(currentIndex);
   ui->jobListWidget->insertItem(newIndex, item);
   ui->jobListWidget->setCurrentRow(newIndex);
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
   const int currentIndex = ui->jobListWidget->currentRow();
   QListWidgetItem* item = ui->jobListWidget->takeItem(currentIndex);
   delete item;
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
