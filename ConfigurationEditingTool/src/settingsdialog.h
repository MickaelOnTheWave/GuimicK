#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

#include "client.h"
#include "SelfIdentity.h"

namespace Ui {
   class SettingsDialog;
}

class SettingsDialog : public QDialog
{
   Q_OBJECT

public:
   explicit SettingsDialog(Client* _client, SelfIdentity* _agent,
                           QWidget *parent = 0);
   ~SettingsDialog();

private slots:
   void on_exitButtonBox_accepted();

   void on_exitButtonBox_rejected();

private:
   void SetDefaultValues();
   void UpdateAgentFromUi();
   void UpdateClientFromUi();
   void UpdateUiFromAgent();
   void UpdateUiFromClient();

   Ui::SettingsDialog *ui;

   Client* client = nullptr;
   SelfIdentity* agent = nullptr;
};

#endif // SETTINGSDIALOG_H
