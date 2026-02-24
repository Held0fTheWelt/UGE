// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#include "CoreContextEffectsLibraryFactory.h"
#include "UObject/SavePackage.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CoreContextEffectsLibraryFactory)

#define LOCTEXT_NAMESPACE "CoreContextEffectsLibraryFactory"

UCoreContextEffectsLibraryFactory::UCoreContextEffectsLibraryFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	// Load at runtime so we don't require GameFeature_FeedbackSystemRuntime to be present
	SupportedClass = LoadObject<UClass>(nullptr, TEXT("/Script/GameFeature_FeedbackSystemRuntime.ContextEffectsLibrary"));
}

FText UCoreContextEffectsLibraryFactory::GetDisplayName() const
{
	return LOCTEXT("ContextEffectsLibraryDisplayName", "Context Effects Library");
}

#undef LOCTEXT_NAMESPACE

UObject* UCoreContextEffectsLibraryFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return (Class ? NewObject<UObject>(InParent, Class, Name, Flags) : nullptr);
}
