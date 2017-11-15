#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "abstractdisplay.h"
#include "abstracteditjobdialog.h"
#include "jobdatamodel.h"
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

   void on_jobListView_doubleClicked(const QModelIndex &index);

private:
   void OpenStandardFile();
   void OpenFile(const QString& filename,
                 const bool showStatusIfOk);
   void UpdateJobListWidget();

   void InsertNewJob(AbstractJob* job);

   void MoveItem(const int currentIndex, const int newIndex);
   void MoveDelegates(const int currentIndex, const int newIndex);

   void ForceJobListViewUpdate();

   bool IsOutOfBounds(const int index) const;

   void UpdateRowDelegatesFromTop(const int startingIndex);
   void UpdateRowDelegatesFromBottom(const int startingIndex);

   AbstractEditJobDialog* CreateEditDialog(AbstractJob* job) const;
   AbstractDisplay* CreateDisplay(AbstractJob* job) const;

   Ui::MainWindow *ui;
   Model model;
   JobDataModel jobListModel;
};

#endif // MAINWINDOW_H
