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

private:
   void SetDefaultValues();

   void UpdateConfigurationFromUi();
   void UpdateAgentFromUi();
   void UpdateClientFromUi();
   void UpdateReportTypeFromUi();
   void UpdateReportDispatchingFromUi();

   void UpdateUiFromConfiguration();
   void UpdateUiFromAgent();
   void UpdateUiFromClient();
   void UpdateReportTypeFromConfiguration();
   void UpdateReportDispatchingFromConfiguration();

   Ui::SettingsDialog *ui;

   StandaloneConfiguration* configuration = nullptr;
};

#endif // SETTINGSDIALOG_H
