#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
   QMainWindow(parent),
   ui(new Ui::MainWindow),
   newJobActions(this)
{
   ui->setupUi(this);

   InstallJobActions();
   connect(&newJobActions, SIGNAL(triggered(QAction*)), this, SLOT(OnAddJob(QAction*)));
}

MainWindow::~MainWindow()
{
   delete ui;
}

void MainWindow::on_actionNew_triggered()
{
   // do stuff
}

void MainWindow::on_actionOpen_triggered()
{
   QFileDialog dialog;
   dialog.exec();
}

void MainWindow::on_actionSave_triggered()
{
}

void MainWindow::on_actionQuit_triggered()
{
   close();
}

void MainWindow::OnAddJob(QAction* action)
{
   const QString jobName = action->text();
   ui->jobView->addItem(jobName);
}

void MainWindow::on_actionAbout_triggered()
{

}

void MainWindow::InstallJobActions()
{
   newJobActions.addAction(ui->actionWake);
   newJobActions.addAction(ui->actionShutdown);
   newJobActions.addAction(ui->actionBackup);
   newJobActions.addAction(ui->actionDisk_space_check);
   newJobActions.addAction(ui->actionConsole);
}
