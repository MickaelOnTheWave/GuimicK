#ifndef SETTINGSUPDATER_H
#define SETTINGSUPDATER_H

#include "settingsdialog.h"

class SettingsUpdater
{
protected:
	SettingsUpdater(Ui::SettingsDialog* _ui,
	                StandaloneConfiguration* _configuration);
	virtual ~SettingsUpdater() = default;

	void UpdateAll();

	Ui::SettingsDialog *ui;
	StandaloneConfiguration* configuration;

private:
	virtual void UpdateAgentData() = 0;
	virtual void UpdateClientData() = 0;
	virtual void UpdateReportType() = 0;
	virtual void UpdateReportDispatching() = 0;
	virtual void UpdateOtherData() = 0;
};

#endif // SETTINGSUPDATER_H
