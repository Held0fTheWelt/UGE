// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/EngineControlUnitComponent.h"

#include "Interfaces/AssetTypeInterface.h"
#include "Interfaces/Control/EngineControlInterface.h"

#include "Libs/CoreInformationFunctionLibrary.h"

#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"

#include "Structs/InformationSet.h"

DEFINE_LOG_CATEGORY(Log_EngineControlUnit);

UEngineControlUnitComponent::UEngineControlUnitComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	GameFeatureName = "EngineControl";
}

void UEngineControlUnitComponent::BeginPlay()
{
	Super::BeginPlay();

	if (IsGameFeatureIgnored())
	{
		return;
	}

	BindEngineControl();
}

void UEngineControlUnitComponent::SetupLoadedAsset(UObject* InOwner, UObject* LoadedObject)
{
	if (IsGameFeatureIgnored())
	{
		return;
	}

	if (!InOwner)
	{
		UE_LOGFMT(Log_EngineControlUnit, Error, "{Time}: {Line} {Class}: Owner is null, cannot setup loaded asset",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	if (!LoadedObject)
	{
		UE_LOGFMT(Log_EngineControlUnit, Error, "{Time}: {Line} {Class}: Loaded object is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	if (Cast<UActorComponent>(InOwner) != this)
	{
		UE_LOGFMT(Log_EngineControlUnit, VeryVerbose, "{Time}: {Line} {Class}: Owner {Owner} is not this component. LoadedObjectName {Name}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Owner", InOwner->GetFName()),
			("Name", LoadedObject->GetFName()));
	}

	if (!LoadedObject->GetClass()->ImplementsInterface(UAssetTypeInterface::StaticClass()))
	{
		UE_LOGFMT(Log_EngineControlUnit, Error, "{Time}: {Line} {Class}: Loaded object does not implement UAssetTypeInterface.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	FPrimaryAssetType PrimaryAssetId = IAssetTypeInterface::Execute_GetAssetType(LoadedObject);

	//if (PrimaryAssetId == IAssetTypeInterface::Execute_GetAssetType(AActor::StaticClass()))
	//{

	//}

	Super::SetupLoadedAsset(InOwner, LoadedObject);
}

void UEngineControlUnitComponent::BindEngineControl()
{
	TArray<UActorComponent*> Components = GetOwner()->GetComponentsByInterface(UEngineControlInterface::StaticClass());

	if (Components.Num() == 0)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
			{
				// Retry seeking camera after a short delay
				UE_LOGFMT(Log_EngineControlUnit, Verbose, "{Time}: {Line} {Class}: No components implementing UEngineControlInterface found, retrying...",
					("Class", GET_CLASSNAME_WITH_FUNCTION),
					("Line", GET_LINE_NUMBER),
					("Time", GET_CURRENT_TIME));
				BindEngineControl();
			});
		return;
	}

	IEngineControlInterface* Interface = Components.Num() > 0 ? Cast<IEngineControlInterface>(Components[0]) : nullptr;

	if (!Interface)
	{
		UE_LOGFMT(Log_EngineControlUnit, Warning, "{Time}: {Line} {Class}: Owner does not implement IEngineControlInterface",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	FNotifyEngineActiveState& EngineEvent = Interface->GetNotifyEngineActiveState();
	EngineEvent.AddDynamic(this, &UEngineControlUnitComponent::OnEngineActiveStateChanged);

	if (EngineEvent.IsAlreadyBound(this, &UEngineControlUnitComponent::OnEngineActiveStateChanged))
	{
		bIsEngineControlActive = true;

		UE_LOGFMT(Log_EngineControlUnit, Log, "{Time}: {Line} {Class}: EngineEvent delegate bound to GetNotifyEngineActiveState. bIsEngineControlActive is true",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
	}
	else
	{
		UE_LOGFMT(Log_EngineControlUnit, Warning, "{Time}: {Line} {Class}: EngineEvent delegate is not bound to GetNotifyEngineActiveState . bIsEngineControlActive is false",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));

		bIsEngineControlActive = false;
	}

	FNotifySecondEngineActiveState& SecondEngineEvent = Interface->GetNotifySecondEngineActiveState();
	SecondEngineEvent.AddDynamic(this, &UEngineControlUnitComponent::OnSecondEngineActiveStateChanged);

	if (SecondEngineEvent.IsAlreadyBound(this, &UEngineControlUnitComponent::OnSecondEngineActiveStateChanged))
	{
		bIsSecondEngineActive = true;

		UE_LOGFMT(Log_EngineControlUnit, Log, "{Time}: {Line} {Class}: SecondEngineEvent delegate bound to GetNotifySecondEngineActiveState. bIsSecondEngineActive is true on Actor {Actor}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Actor", GetOwner()->GetFName()));
	}
	else
	{
		UE_LOGFMT(Log_EngineControlUnit, Warning, "{Time}: {Line} {Class}: EngineEvent delegate is not bound to GetNotifySecondEngineActiveState . bIsSecondEngineActive is false",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));

		bIsEngineControlActive = false;
	}
}


void UEngineControlUnitComponent::OnEngineActiveStateChanged(bool IsOn)
{
	bIsEngineActive = IsOn;

	UE_LOGFMT(Log_EngineControlUnit, Verbose, "{Time}: {Line} {Class}: GetNotifyEngineActiveState called with IsOn: {IsOn}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("IsOn", IsOn));
}

void UEngineControlUnitComponent::OnSecondEngineActiveStateChanged(bool IsOn)
{
	bIsSecondEngineActive = IsOn;

	UE_LOGFMT(Log_EngineControlUnit, Verbose, "{Time}: {Line} {Class}: GetNotifySecondEngineActiveState called with IsOn: {IsOn}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("IsOn", IsOn));
}

TArray<FInformationSet> UEngineControlUnitComponent::GetSettingsVariableSets_Implementation() const
{
	TArray<FInformationSet> InformationSets = TArray<FInformationSet>();

	FInformationSet EngineControlUnitInfoSet = UCoreInformationFunctionLibrary::GetInformationSet(
		"Engine Control Unit",
		"",
		""
	);

	UCoreInformationFunctionLibrary::AddInformationValue(
		EngineControlUnitInfoSet,
		"EngineControlActive",
		bIsEngineControlActive ? "True" : "False",
		EValueType::VT_BOOL
	);

	UCoreInformationFunctionLibrary::AddInformationValue(
		EngineControlUnitInfoSet,
		"1st EngineActive",
		bIsEngineActive ? "True" : "False",
		EValueType::VT_BOOL
	);

	UCoreInformationFunctionLibrary::AddInformationValue(
		EngineControlUnitInfoSet,
		"2nd EngineActive",
		bIsSecondEngineActive ? "True" : "False",
		EValueType::VT_BOOL
	);

	InformationSets.Add(EngineControlUnitInfoSet);

	InformationSets.Append(Super::GetSettingsVariableSets_Implementation());

	return InformationSets;
}