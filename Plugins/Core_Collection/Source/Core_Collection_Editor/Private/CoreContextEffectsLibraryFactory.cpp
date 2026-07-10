// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for UGE

#include "CoreContextEffectsLibraryFactory.h"
#include "UObject/SavePackage.h"
#include "UObject/UObjectGlobals.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CoreContextEffectsLibraryFactory)

#define LOCTEXT_NAMESPACE "CoreContextEffectsLibraryFactory"

UCoreContextEffectsLibraryFactory::UCoreContextEffectsLibraryFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	// Optional GameFeature class; use only when the module is already loaded.
	SupportedClass = FindObject<UClass>(nullptr, TEXT("/Script/GameFeature_FeedbackSystemRuntime.ContextEffectsLibrary"));
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
