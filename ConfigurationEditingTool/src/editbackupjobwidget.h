#ifndef EDITBACKUPJOBWIDGET_H
#define EDITBACKUPJOBWIDGET_H

#include <QWidget>

#include "abstractbackupjob.h"
#include "pathselectioncontrol.h"

namespace Ui {
   class EditBackupJobWidget;
}

class EditBackupJobWidget : public QWidget
{
   Q_OBJECT

public:
   explicit EditBackupJobWidget(QWidget *parent = nullptr);
   virtual ~EditBackupJobWidget();

   void SetupDestinationAsFile(const QString& message, const QString& fileFilter);

   void UpdateUiFromJob(AbstractBackupJob* job);
   void UpdateJobFromUi(AbstractBackupJob* job) const;

protected:
   virtual void FinishedDestinationEditing(const QString& value);

   void SetDestinationWidgetPath(const QString& value);

private slots:
   void on_addBackupPointButton_clicked();

   void on_removeBackupPointButton_clicked();

   void on_backupPointsWidget_itemSelectionChanged();

   void OnFinishedSourceEditing(const QString& value);
   void OnFinishedDestinationEditing(const QString& value);

private:
   void AddBackupPointsToUi(
      const std::vector<std::pair<std::wstring,std::wstring> >& backupPoints
   );
   void AddBackupPointsToJob(AbstractBackupJob* job) const;

   void SetFolderWidgetValue(PathSelectionControl* widget,
                             const int rowIndex, const int columnIndex);

   void InitializeControls();
   void EnableControls(const bool value);

   void OnFinishedPathEditing(const QString& value,
                              const int columnIndex);

   Ui::EditBackupJobWidget *ui;
};

#endif // EDITBACKUPJOBWIDGET_H
