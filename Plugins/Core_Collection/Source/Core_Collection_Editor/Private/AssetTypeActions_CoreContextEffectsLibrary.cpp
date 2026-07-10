// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for UGE

#include "AssetTypeActions_CoreContextEffectsLibrary.h"
#include "UObject/UnrealType.h"
#include "UObject/UObjectGlobals.h"

UClass* FAssetTypeActions_CoreContextEffectsLibrary::GetSupportedClass() const
{
	// Optional GameFeature class; use only when the module is already loaded.
	return FindObject<UClass>(nullptr, TEXT("/Script/GameFeature_FeedbackSystemRuntime.ContextEffectsLibrary"));
}
