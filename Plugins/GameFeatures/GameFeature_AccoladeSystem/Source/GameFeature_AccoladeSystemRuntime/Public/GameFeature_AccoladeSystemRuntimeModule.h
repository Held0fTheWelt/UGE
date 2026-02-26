#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FGameFeature_AccoladeSystemRuntimeModule : public IModuleInterface
{
public:
	//~IModuleInterface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	//~End of IModuleInterface
};
