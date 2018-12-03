#include "settingsupdater.h"

SettingsUpdater::SettingsUpdater(Ui::SettingsDialog* _ui,
                                 StandaloneConfiguration* _configuration)
   : ui(_ui), configuration(_configuration)
{
}

void SettingsUpdater::UpdateAll()
{
	UpdateAgentData();
	UpdateClientData();
	UpdateReportType();
	UpdateReportDispatching();
	UpdateOtherData();
}
