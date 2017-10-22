#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QActionGroup>
#include <QMainWindow>

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
   void on_actionQuit_triggered();
   void OnAddJob(QAction* action);

   void on_actionNew_triggered();

   void on_actionOpen_triggered();

   void on_actionSave_triggered();

   void on_actionAbout_triggered();

private:
   void InstallJobActions();

   Ui::MainWindow *ui;
   QActionGroup newJobActions;
};

#endif // MAINWINDOW_H
