// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class UWorld;
class UClass;

/**
 * Provider for development/editor-only helpers (PIE world, class lookup by name).
 * Implemented by Core_Collection; GameFeatures use this via the registry so they do not depend on Core.
 */
class ELEMENTAL_INTERFACES_API IDevelopmentStaticsProvider
{
public:
	virtual ~IDevelopmentStaticsProvider() = default;

	/** Finds the PIE authority world (listen server or dedicated server in editor). Returns nullptr if not in PIE. */
	virtual UWorld* GetPlayInEditorAuthorityWorld() = 0;

	/** Finds a class by short name (e.g. for cheat console). Returns nullptr if not found. */
	virtual UClass* FindClassByShortName(const FString& SearchToken, UClass* DesiredBaseClass, bool bLogFailures = true) = 0;
};

/** Registry so Core can register its implementation and GameFeatures can resolve it without depending on Core. */
namespace DevelopmentStaticsProviderRegistry
{
	/** Set the provider (e.g. from Core_System module startup). Pass nullptr to clear. */
	ELEMENTAL_INTERFACES_API void Set(IDevelopmentStaticsProvider* Provider);

	/** Get the provider. Returns nullptr if none registered. */
	ELEMENTAL_INTERFACES_API IDevelopmentStaticsProvider* Get();
}
