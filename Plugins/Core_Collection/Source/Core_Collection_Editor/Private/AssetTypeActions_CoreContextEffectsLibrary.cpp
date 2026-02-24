// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#include "AssetTypeActions_CoreContextEffectsLibrary.h"
#include "UObject/UnrealType.h"

UClass* FAssetTypeActions_CoreContextEffectsLibrary::GetSupportedClass() const
{
	// Load at runtime so we don't require GameFeature_FeedbackSystemRuntime to be present
	return LoadObject<UClass>(nullptr, TEXT("/Script/GameFeature_FeedbackSystemRuntime.ContextEffectsLibrary"));
}
