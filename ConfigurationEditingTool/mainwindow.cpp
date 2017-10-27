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

