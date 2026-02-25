// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(Log_CoreEffects, Log, All);

/**
 * Core_Effects Module
 * 
 * Module for Gameplay Effects and related functionality.
 */
class CORE_EFFECTS_API FCoreEffectsModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
