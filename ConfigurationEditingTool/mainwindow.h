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

   void on_deleteButton_clicked();

private:
   void UpdateJobListWidget();

   Ui::MainWindow *ui;
   Model model;
};

#endif // MAINWINDOW_H
