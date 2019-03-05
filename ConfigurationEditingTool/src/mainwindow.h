#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "abstractbackupjob.h"
#include "abstractdisplay.h"
#include "abstracteditjobdialog.h"
#include "configurationtype.h"
#include "jobdatamodel.h"
#include "tooledconfiguration.h"

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

   void OpenDefaultFile();

protected:
   void closeEvent(QCloseEvent *event) override;

private slots:
   void on_actionNew_triggered();

   void on_actionOpen_triggered();

   void on_actionSave_triggered();

   void on_actionSave_As_triggered();

   void on_actionQuit_triggered();

   void on_actionGeneral_triggered();

   void on_actionAbout_triggered();

   void on_upButton_clicked();

   void on_downButton_clicked();

   void on_deleteButton_clicked();

   void on_jobListView_doubleClicked(const QModelIndex &index);

   void on_jobListView_clicked(const QModelIndex &index);

   void on_checkBackupsButton_clicked();

   void on_actionRun_triggered();

   void on_actionTask_Tool_triggered();

private:
   void MoveToScreenCenter();

   void SetupAddJobMenu();
   void SetupWindowsAddJobMenu();
   void SetupLinuxAddJobMenu();

   void AddJobMenuEntry(const QString& title,
                        AbstractJob* job);

   void AddJobMenuEntry(QMenu* subMenu,
                        const QString& title,
                        AbstractJob* job);

   bool ShouldDiscardCurrentChanges();

   void CreateNewFile();
   void OpenFile(const QString& filename,
                 const bool showStatusIfOk);
   void SaveFile(const QString& filename);
   void SaveConfigurationToFile(TooledConfiguration& customConfig,
                                const QString& filename);
   void QuitApplication();

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

   ConfigurationType ChooseConfigurationType() const;

   void UpdateModificationStatus();

   QString CreateWindowTitle() const;

   void UpdateUiOnFileChange(const QString& newFile);

   void UpdateAddJobMenuEntries();

   void OpenRunDialog(const std::string& reportFile);

   QString GetTempFolder() const;
   QString GetTempConfigFilename() const;
   QString GetTempReportFolder() const;
   std::string SaveConfigurationToTempLocation();

   Ui::MainWindow *ui;
   TooledConfiguration model;
   JobDataModel jobListModel;
   ConfigurationType configurationType;
   bool restrictToStandaloneMode = false;

   bool hasConfigurationChanged = false;
   QString currentConfigurationFile = "";
};

#endif // MAINWINDOW_H
