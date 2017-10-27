#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "model.h"

namespace Ui {
   class MainWindow;
}

class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
   explicit MainWindow(QWidget *parent = 0);
   ~MainWindow();

private slots:
   void on_actionNew_triggered();

   void on_actionOpen_triggered();

   void on_actionSave_triggered();

   void on_actionClose_triggered();

   void on_upButton_clicked();

   void on_downButton_clicked();

   void on_addButton_clicked();

   void on_deleteButton_clicked();

   void on_actionWake_triggered();

   void on_actionShutdown_triggered();

   void on_actionBackup_triggered();

   void on_actionCustom_command_triggered();

private:
   void OpenStandardFile();
   void OpenFile(const QString& filename,
                 const bool showStatusIfOk);
   void UpdateJobListWidget();

   void InsertNewJob(const QString& name);

   Ui::MainWindow *ui;
   Model model;
};

#endif // MAINWINDOW_H
