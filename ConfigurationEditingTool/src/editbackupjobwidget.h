#ifndef EDITBACKUPJOBWIDGET_H
#define EDITBACKUPJOBWIDGET_H

#include <QWidget>

#include "abstractbackupjob.h"

namespace Ui {
   class EditBackupJobWidget;
}

class EditBackupJobWidget : public QWidget
{
   Q_OBJECT

public:
   explicit EditBackupJobWidget(QWidget *parent = 0);
   ~EditBackupJobWidget();

   void UpdateUiFromJob(AbstractBackupJob* job);
   void UpdateJobFromUi(AbstractBackupJob* job) const;

private slots:
   void on_addBackupPointButton_clicked();

   void on_removeBackupPointButton_clicked();

   void on_backupPointsWidget_itemSelectionChanged();

private:
   void AddBackupPointsToUi(
      const std::vector<std::pair<std::string,std::string> >& backupPoints
   );
   void AddBackupPointsToJob(AbstractBackupJob* job) const;

   Ui::EditBackupJobWidget *ui;
};

#endif // EDITBACKUPJOBWIDGET_H
