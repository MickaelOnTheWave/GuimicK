#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "abstractbackupjob.h"
#include "abstractdisplay.h"
#include "abstracteditjobdialog.h"
#include "configurationtype.h"
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

   void RestrictToStandaloneMode();

private slots:
   void on_actionNew_triggered();

   void on_actionOpen_triggered();

   void on_actionSave_triggered();

   void on_actionSave_As_triggered();

   void on_actionQuit_triggered();

   void on_upButton_clicked();

   void on_downButton_clicked();

   void on_deleteButton_clicked();

   void on_actionWake_triggered();

   void on_actionShutdown_triggered();

   void on_actionCustom_command_triggered();

   void on_jobListView_doubleClicked(const QModelIndex &index);

   void on_actionRaw_Copy_triggered();

   void on_actionRsync_Copy_triggered();

   void on_actionGit_Filesystems_triggered();

   void on_actionGit_Repositories_triggered();

   void on_actionRsnapshot_triggered();

   void on_actionZip_Copy_triggered();

   void on_actionDisk_space_check_triggered();

   void on_actionAbout_triggered();

   void on_actionCustom_command_client_triggered();

   void on_jobListView_clicked(const QModelIndex &index);

   void on_checkBackupsButton_clicked();

private:
   void MoveToScreenCenter();

   void OpenStandardFile();
   void OpenFile(const QString& filename,
                 const bool showStatusIfOk);
   void SaveFile(const QString& filename);

   void UpdateJobListWidget();

   void InsertNewJob(AbstractJob* job);

   void MoveItem(const int currentIndex, const int newIndex);
   void MoveDelegates(const int currentIndex, const int newIndex);

   void ForceJobListViewUpdate();

   bool IsOutOfBounds(const int index) const;

   void UpdateRowDelegatesFromTop(const int startingIndex);
   void UpdateRowDelegatesFromBottom(const int startingIndex);

   AbstractDisplay* CreateDisplay(AbstractJob* job) const;
   void CreateJobDisplayDelegates(const std::list<AbstractJob*>& jobs);

   QString GetBackupFolder(AbstractBackupJob* job) const;

   int GetBackupTimeIndex(AbstractBackupJob* job, const QString& backupFolder) const;

   void RestoreBackup(AbstractBackupJob* job, const QString& backupFolder, const int timeIndex) const;

   AbstractTypeConfiguration* CreateConfigurationManager();

   ConfigurationType ChooseConfigurationType() const;

   void UpdateModificationStatus();

   QString CreateWindowTitle() const;

   Ui::MainWindow *ui;
   Model model;
   JobDataModel jobListModel;
   ConfigurationType configurationType;
   bool restrictToStandaloneMode = false;

   bool hasConfigurationChanged = false;
   QString currentConfigurationFile = "";
};

#endif // MAINWINDOW_H
