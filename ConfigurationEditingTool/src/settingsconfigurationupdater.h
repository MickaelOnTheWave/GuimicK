#ifndef SETTINGSCONFIGURATIONUPDATER_H
#define SETTINGSCONFIGURATIONUPDATER_H

#include "settingsupdater.h"

class SettingsConfigurationUpdater : public SettingsUpdater
{
public:
   static void Update(Ui::SettingsDialog* ui, StandaloneConfiguration* configuration);

private:
   SettingsConfigurationUpdater(Ui::SettingsDialog* _ui, StandaloneConfiguration* _configuration);

   void UpdateAgentData() override;
   void UpdateClientData() override;
   void UpdateReportType() override;
   void UpdateReportDispatching() override;
   void UpdateOtherData() override;

   void Update7zipPathProperty(const QString& value);

   EmailData CreateEmailData();
};


#endif // SETTINGSCONFIGURATIONUPDATER_H
