// © Felix Courteau-Boutin All Right Reserved

#pragma once

#include "Modules/ModuleManager.h"

class FSimpleQuestSystemModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
