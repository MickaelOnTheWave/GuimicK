#ifndef SETTINGSUIUPDATER_H
#define SETTINGSUIUPDATER_H

#include "settingsupdater.h"

class SettingsUiUpdater : public SettingsUpdater
{
public:
   static void Update(Ui::SettingsDialog* ui, StandaloneConfiguration* configuration);

private:
   SettingsUiUpdater(Ui::SettingsDialog* _ui, StandaloneConfiguration* _configuration);

   void UpdateAgentData() override;
   void UpdateClientData() override;
   void UpdateReportType() override;
   void UpdateReportDispatching() override;
   void UpdateOtherData() override;

   void UpdateEmailData(const EmailAccountData& emailData);
   void SetReportDispatchControls(const int index);
};

#endif // SETTINGSUIUPDATER_H
