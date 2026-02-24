// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#include "CoreRuntimeOptions.h"
#include "UObject/Class.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CoreRuntimeOptions)

UCoreRuntimeOptions::UCoreRuntimeOptions()
{
	OptionCommandPrefix = TEXT("ro");
}

UCoreRuntimeOptions* UCoreRuntimeOptions::GetRuntimeOptions()
{
	return GetMutableDefault<UCoreRuntimeOptions>();
}

const UCoreRuntimeOptions& UCoreRuntimeOptions::Get()
{
	const UCoreRuntimeOptions& RuntimeOptions = *GetDefault<UCoreRuntimeOptions>();
	return RuntimeOptions;
}
