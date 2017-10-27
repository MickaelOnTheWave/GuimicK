#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <string>
#include <vector>
#include <QFileDialog>

#include "configurationcheckdialog.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
   QMainWindow(parent),
   ui(new Ui::MainWindow)
{
   ui->setupUi(this);
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
   {
      vector<string> errors;
      bool isUsable = model.LoadConfiguration(filename.toStdString(), errors);
      ConfigurationCheckDialog::Show(!isUsable, errors);
      if (isUsable)
         UpdateJobListWidget();
   }
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

void MainWindow::on_deleteButton_clicked()
{
   const int currentIndex = ui->jobListWidget->currentRow();
   QListWidgetItem* item = ui->jobListWidget->takeItem(currentIndex);
   delete item;
}
