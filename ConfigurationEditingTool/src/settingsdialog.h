#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

#include "client.h"
#include "standaloneconfiguration.h"

namespace Ui {
   class SettingsDialog;
}

class SettingsDialog : public QDialog
{
   Q_OBJECT

public:
   explicit SettingsDialog(StandaloneConfiguration* _configuration,
                           QWidget *parent = 0);
   ~SettingsDialog();

private slots:
   void on_exitButtonBox_accepted();

   void on_exitButtonBox_rejected();

   void on_reportFormatBox_currentIndexChanged(const QString &arg1);

   void on_reportDispatchBox_currentIndexChanged(int index);

private:
   void SetDefaultValues();
   void SetDefaultClientName();
   void SetDefaultAgentName();
   void SetDefaultCss();
   void SetDefaultReportFolder();

   void InitializeCssSelectionWidget();
   void InitializeReportFolderSelectionWidget();
   void Initialize7zipWidget();
   void InitializeReportDispatchBox();

   void HideClientTab();
   void HideDevelopmentFeatures();

   Ui::SettingsDialog *ui;
   StandaloneConfiguration* configuration;
};



#endif // SETTINGSDIALOG_H
