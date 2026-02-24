// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CombinedBundleActorComponent.h"

#include "AssetManager/AssetManagedLoaderHelper.h"
#include "AssetManager/AsyncAssetLoaderHelper.h"

#include "Interfaces/ShouldAddGameFeatureInterface.h"

#include "Helper/AssetLoader/HandleAssetLoaded.h"

#include "Libs/CoreInformationFunctionLibrary.h"

#include "Logging/Core_Classes_Logging.h"
#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"

#include "Structs/InformationSet.h"

#include "TimerManager.h"

#if WITH_EDITOR
static bool bLoadCombinedBundleComponent = true;
static bool bLogCombinedBundleComponent = false;
#endif

UCombinedBundleActorComponent::UCombinedBundleActorComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bLoadPrimaryAssets = true;
}


void UCombinedBundleActorComponent::BeginPlay()
{
    Super::BeginPlay();

	if (GetOwner()->GetClass()->ImplementsInterface(UShouldAddGameFeatureInterface::StaticClass()))
	{
		if (IShouldAddGameFeatureInterface::Execute_ShouldIgnoreGameFeature(GetOwner(), GameFeatureName))
		{
			UE_LOGFMT(Log_GameFeatures, Verbose, "{Time}: {Line} {Class}: {GameFeatureName} GameFeature is ignored by owner {Owner}", ("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME), ("GameFeatureName", GameFeatureName), ("Owner", GetOwner()->GetFName()));
			bIgnoredFeatureOnThisActor = true;
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UCombinedBundleActorComponent::DeferredDestroy, 0.5f, false);
			return;
		}
	}

	if(bLoadPrimaryAssets)
	{
		UE_LOGFMT(Log_CombinedComponents, VeryVerbose, "{Time}: {Line} {Class}: CombinedBundleActorComponent loading primary assets for GameFeature: {GameFeatureName}on Actor {Owner}.", 
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("GameFeatureName", GameFeatureName),
			("Owner", GetOwner()->GetFName()));

		LoadPrimaryAssets();
	}
	else
	{
		UE_LOGFMT(Log_CombinedComponents, Verbose, "{Time}: {Line} {Class}: CombinedBundleActorComponent not primary assets for GameFeature: {GameFeatureName}on Actor {Owner}.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("GameFeatureName", GameFeatureName),
			("Owner", GetOwner()->GetFName()));
	}
}

void UCombinedBundleActorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	//UE_LOGFMT(Log_CombinedPawn, Warning, "{Time}: {Line} {Class}: CombinedWheeledVehiclePawn EndPlay called.",
	//	("Class", GET_CLASSNAME_WITH_FUNCTION),
	//	("Line", GET_LINE_NUMBER),
	//	("Time", GET_CURRENT_TIME));
	// Unbind static delegates
	UnbindAssetLoaderEventListener();
}

void UCombinedBundleActorComponent::DeferredDestroy()
{
	UE_LOGFMT(Log_GameFeatures, VeryVerbose, "{Time}: {Line} {Class}: Ignored GameFeature on this Actor. Destroying component.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME));

	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	DestroyComponent();
}

void UCombinedBundleActorComponent::SetEntityCoreLoggingEnabled_Implementation(const bool bEnabled)
{
	bLogCoreInformation = bEnabled;
	UE_LOGFMT(Log_CombinedComponents, VeryVerbose, "{Time}: {Line} {Class}: SetEntity Core Logging Enabled: {bEnabled}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("bEnabled", bEnabled ? TEXT("True") : TEXT("False")));
}

FName UCombinedBundleActorComponent::GetCoreInformationName_Implementation() const
{
	return FName(GameFeatureName);
}

TArray<FInformationSet> UCombinedBundleActorComponent::GetSettingsVariableSets_Implementation() const
{
	TArray<FInformationSet> InformationSets;

	FInformationSet DefaultSet = UCoreInformationFunctionLibrary::GetInformationSet(
		"AssetLoader",
		"",
		""
	);

	UCoreInformationFunctionLibrary::AddInformationValue(
		DefaultSet,
		"LoadPrimaryAssets",
		bLoadPrimaryAssets ? "True" : "False",
		EValueType::VT_BOOL
	);

	InformationSets.Add(DefaultSet);

	return InformationSets;
}

bool UCombinedBundleActorComponent::IsGameFeatureIgnored() const
{
	return bIgnoredFeatureOnThisActor;
}

void UCombinedBundleActorComponent::LoadPrimaryAssets()
{
#if WITH_EDITOR
	if (!bLoadCombinedBundleComponent)
	{
		UE_LOGFMT(Log_CombinedActors, Warning, "{Time}: {Line} {Class}: LoadPrimaryAssets() is disabled in editor settings.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}
#endif
	// --- Binding der statischen Delegates ---
	UE_LOGFMT(Log_CombinedComponents, VeryVerbose, "{Time}: {Line} {Class}: LoadPrimaryAssets() called. {Name}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Name", GetFName().ToString()));

	UE_LOGFMT(Log_CombinedComponents, VeryVerbose, "{Time}: {Line} {Class}: DataAsset Identifier: {DataAssetIdentifier}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("DataAssetIdentifier", GameFeatureName));

	// --- Binding der statischen Delegates ---
	UE_LOGFMT(Log_CombinedComponents, VeryVerbose, "{Time}: {Line} {Class}: Binding static delegates.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME));
	// --- Binding der statischen Delegates ---
	AssetLoaderEvents.OnPrimaryAssetClassNotFound.AddDynamic(
		this, &UCombinedBundleActorComponent::OnDefinitionAssetNotFound);
	AssetLoaderEvents.OnPrimaryAssetLoaded.AddDynamic(
		this, &UCombinedBundleActorComponent::SetupLoadedAsset);

	// Kick off
	UE_LOGFMT(Log_CombinedComponents, VeryVerbose, "{Time}: {Line} {Class}: Starting to load actor definition config asset. {Owner}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Owner", GetFName().ToString()));

	UAssetManagedLoaderHelper::LoadDefinitionConfigAssets(
		AssetLoaderEvents,
		EAssetDefinitionType::ADT_COMPONENT,
		this,
		bComponentHasComponentAssetToLoad,
		bComponentHasActorAssetToLoad,
		bDebugRoute
	);
}
void UCombinedBundleActorComponent::UnbindAssetLoaderEventListener()
{
	// Unbind static delegates
	UE_LOGFMT(Log_CombinedComponents, VeryVerbose, "{Time}: {Line} {Class}: Unbinding static delegates.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME));
	AssetLoaderEvents.OnPrimaryAssetClassNotFound.RemoveDynamic(
		this, &UCombinedBundleActorComponent::OnDefinitionAssetNotFound);
	AssetLoaderEvents.OnPrimaryAssetLoaded.RemoveDynamic(
		this, &UCombinedBundleActorComponent::SetupLoadedAsset);
}

void UCombinedBundleActorComponent::OnDefinitionAssetNotFound(UObject* InOwner, bool bComponentHasClassesToLoad)
{
	if (!InOwner)
	{
		UE_LOGFMT(Log_CombinedComponents, Warning, "{Time}: {Line} {Class}: Owner is null, cannot process loaded asset.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	if (InOwner != this)
	{
		return;
	}

	if (bComponentHasClassesToLoad)
	{
		UE_LOGFMT(Log_CombinedComponents, Verbose, "{Time}: {Line} {Class}: Definition asset not found or failed to load, but component has classes to load.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
	}
	else
	{
		UE_LOGFMT(Log_CombinedComponents, Verbose, "{Time}: {Line} {Class}: Definition asset not found or failed to load, and component has no classes to load.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
	}
}

void UCombinedBundleActorComponent::SetupLoadedAsset(UObject* InOwner, UObject* LoadedObject)
{
	if (!InOwner)
	{
		UE_LOGFMT(Log_CombinedComponents, Error, "{Time}: {Line} {Class}: Owner is null, cannot setup loaded asset",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	if (!LoadedObject)
	{
		UE_LOGFMT(Log_CombinedComponents, Error, "{Time}: {Line} {Class}: Loaded object is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	if (Cast<UActorComponent>(InOwner) != this)
	{
		UE_LOGFMT(Log_CombinedComponents, VeryVerbose, "{Time}: {Line} {Class}: Owner {Owner} is not this component. LoadedObjectName {Name}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Owner", InOwner->GetFName()),
			("Name", LoadedObject->GetFName()));
	}

	UE_LOGFMT(Log_CombinedComponents, VeryVerbose, "{Time}: {Line} {Class}: Owner {Owner} loaded Object Name {Name}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Owner", InOwner->GetFName()),
		("Name", LoadedObject->GetFName()));
}

FName UCombinedBundleActorComponent::GetDefinitionAssetName_Implementation() const
{
	return FName(GameFeatureName);
}