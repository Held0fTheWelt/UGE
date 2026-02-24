// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/StaticMeshHierarchyAnimationComponent.h"

#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "HAL/IConsoleManager.h"
#include "PhysicsEngine/BodySetup.h"
#include "Interfaces/AssetTypeInterface.h"
#include "Interfaces/StaticMeshComposingInterface.h"
#include "Interfaces/Control/EngineControlInterface.h"

#include "Libs/CoreInformationFunctionLibrary.h"

#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"

#include "Structs/InformationSet.h"

DEFINE_LOG_CATEGORY(Log_StaticMeshAnimator);

static TAutoConsoleVariable<int32> CVarSMADrawCOMVerbose(
	TEXT("sma.DrawCOMVerbose"), 0,
	TEXT("SMA COM debug: 1 = show positions in debug text"));

namespace StaticMeshHierarchyAnimatorDebug
{
	static bool bDebugMassesAndCOM = false;
}

/** Get the individual mass of a static mesh component from its BodySetup.
 *  GetMass() returns the welded root body mass for non-simulating components,
 *  so all meshes in a hierarchy would report the same (total) mass.
 *  BodySetup::CalculateMass() computes mass from this mesh's collision geometry,
 *  scale, and physical material density. */
static float GetIndividualMeshMass(UStaticMeshComponent* SMC)
{
	if (UBodySetup* BS = SMC->GetBodySetup())
	{
		return BS->CalculateMass(SMC);
	}
	return 0.f;
}

void UStaticMeshHierarchyAnimationComponent::SetDebugMassesAndCOMEnabled_Implementation(bool bEnabled)
{
	StaticMeshHierarchyAnimatorDebug::bDebugMassesAndCOM = bEnabled;
	UE_LOGFMT(Log_StaticMeshAnimator, Log, "{Time}: {Line} {Class}: Debug masses/COM {State}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("State", bEnabled ? TEXT("enabled") : TEXT("disabled")));
}

UStaticMeshHierarchyAnimationComponent::UStaticMeshHierarchyAnimationComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	
	bLoadPrimaryAssets = true;
	bComponentHasComponentAssetToLoad = false;
	GameFeatureName = "StaticMeshAnimator";

	StaticMeshComponents.Empty();
	bIsStaticMeshCollectionAvailable = false;
}

void UStaticMeshHierarchyAnimationComponent::SetupLoadedAsset(UObject* InOwner, UObject* LoadedObject)
{
	if (IsGameFeatureIgnored())
	{
		return;
	}

	if (!InOwner)
	{
		UE_LOGFMT(Log_StaticMeshAnimator, Error, "{Time}: {Line} {Class}: Owner is null, cannot setup loaded asset",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	if (!LoadedObject)
	{
		UE_LOGFMT(Log_StaticMeshAnimator, Error, "{Time}: {Line} {Class}: Loaded object is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	if (Cast<UActorComponent>(InOwner) != this)
	{
		UE_LOGFMT(Log_StaticMeshAnimator, VeryVerbose, "{Time}: {Line} {Class}: Owner {Owner} is not this component. LoadedObjectName {Name}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Owner", InOwner->GetFName()),
			("Name", LoadedObject->GetFName()));
	}

	if (!LoadedObject->GetClass()->ImplementsInterface(UAssetTypeInterface::StaticClass()))
	{
		UE_LOGFMT(Log_StaticMeshAnimator, Error, "{Time}: {Line} {Class}: Loaded object does not implement UAssetTypeInterface.",
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

void UStaticMeshHierarchyAnimationComponent::BeginPlay()
{
	Super::BeginPlay();

	if (IsGameFeatureIgnored())
	{
		return;
	}

	BindStaticMeshComposerFinished();

	UE_LOGFMT(Log_StaticMeshAnimator, Verbose, "{Time}: {Line} {Class}: Static Mesh Composing Component has started",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME));

}

void UStaticMeshHierarchyAnimationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (StaticMeshHierarchyAnimatorDebug::bDebugMassesAndCOM && StaticMeshComponents.Num() > 0)
	{
		DrawDebugMassesAndCenterOfMass();
	}
}

void UStaticMeshHierarchyAnimationComponent::DrawDebugMassesAndCenterOfMass()
{
	UWorld* World = GetWorld();
	if (!World || StaticMeshComponents.Num() == 0)
	{
		return;
	}

	FVector WeightedCOM = FVector::ZeroVector;
	FVector UnweightedCOM = FVector::ZeroVector;
	float TotalMass = 0.f;
	int32 MeshCount = 0;
	const bool bVerbose = CVarSMADrawCOMVerbose.GetValueOnGameThread() != 0;
	const float SphereRadius = 8.f;
	const float TotalCOMRadius = 18.f;
	const float DrawDuration = 0.f;

	for (const auto& Pair : StaticMeshComponents)
	{
		UStaticMeshComponent* SMC = Cast<UStaticMeshComponent>(Pair.Value);
		if (!SMC)
		{
			continue;
		}

		// BodySetup::CalculateMass gives the individual mesh mass from collision geometry.
		// GetMass() would return the welded root body total for non-simulating components.
		const float Mass = GetIndividualMeshMass(SMC);
		// Bounds center: geometric center of this mesh part in world space.
		const FVector WorldCenter = SMC->Bounds.Origin;

		if (Mass > 0.f)
		{
			WeightedCOM += Mass * WorldCenter;
			TotalMass += Mass;
		}
		UnweightedCOM += WorldCenter;
		++MeshCount;

		DrawDebugSphere(World, WorldCenter, SphereRadius, 10, FColor::Cyan, false, DrawDuration, 0, 1.5f);
		const FString Label = bVerbose
			? FString::Printf(TEXT("%s: %.1f kg (X=%.1f Y=%.1f Z=%.1f)"), *Pair.Key.ToString(), Mass, WorldCenter.X, WorldCenter.Y, WorldCenter.Z)
			: FString::Printf(TEXT("%s: %.1f kg"), *Pair.Key.ToString(), Mass);
		DrawDebugString(World, WorldCenter + FVector(0, 0, SphereRadius + 15.f), Label, nullptr, FColor::White, 0.f, true, 1.2f);
	}

	// Total COM: prefer the physics body COM (from subclass override), fall back to mesh-weighted or geometric average
	FVector TotalCOM;
	bool bUsedPhysicsCOM = false;
	if (GetPhysicsBodyCenterOfMassWorld(TotalCOM))
	{
		bUsedPhysicsCOM = true;
	}
	else if (TotalMass > 0.f)
	{
		TotalCOM = WeightedCOM / TotalMass;
	}
	else if (MeshCount > 0)
	{
		TotalCOM = UnweightedCOM / static_cast<float>(MeshCount);
		TotalMass = 0.f;
	}
	else
	{
		return;
	}

	DrawDebugSphere(World, TotalCOM, TotalCOMRadius, 14, FColor::Yellow, false, DrawDuration, 0, 2.5f);
	const FString COMSource = bUsedPhysicsCOM ? TEXT("Physics") : TEXT("Mesh");
	const FString TotalLabel = bVerbose
		? FString::Printf(TEXT("COM total [%s]: %.1f kg (X=%.1f Y=%.1f Z=%.1f)"), *COMSource, TotalMass, TotalCOM.X, TotalCOM.Y, TotalCOM.Z)
		: FString::Printf(TEXT("COM total [%s]: %.1f kg"), *COMSource, TotalMass);
	DrawDebugString(World, TotalCOM + FVector(0, 0, TotalCOMRadius + 20.f), TotalLabel, nullptr, FColor::Yellow, 0.f, true, 1.4f);
}

static void ComputeMassesAndCOMs(const TMap<FName, TObjectPtr<USceneComponent>>& StaticMeshComponents,
	FVector& OutTotalCOM, float& OutTotalMass, TArray<FStaticMeshMassAndCOM>* OutPerMesh)
{
	OutTotalCOM = FVector::ZeroVector;
	OutTotalMass = 0.f;
	FVector UnweightedSum = FVector::ZeroVector;
	int32 MeshCount = 0;
	if (OutPerMesh) OutPerMesh->Reset();

	for (const auto& Pair : StaticMeshComponents)
	{
		UStaticMeshComponent* SMC = Cast<UStaticMeshComponent>(Pair.Value);
		if (!SMC) continue;

		const float Mass = GetIndividualMeshMass(SMC);
		// Bounds center: geometric center of this mesh part in world space.
		const FVector WorldCenter = SMC->Bounds.Origin;

		if (Mass > 0.f)
		{
			OutTotalCOM += Mass * WorldCenter;
			OutTotalMass += Mass;
		}
		UnweightedSum += WorldCenter;
		++MeshCount;

		if (OutPerMesh)
		{
			FStaticMeshMassAndCOM Entry;
			Entry.MeshName = Pair.Key;
			Entry.Mass = Mass;
			Entry.WorldCOM = WorldCenter;
			OutPerMesh->Add(Entry);
		}
	}

	if (OutTotalMass > 0.f)
	{
		OutTotalCOM /= OutTotalMass;
	}
	else if (MeshCount > 0)
	{
		// Fallback: geometric average when no mass data available
		OutTotalCOM = UnweightedSum / static_cast<float>(MeshCount);
	}
}

bool UStaticMeshHierarchyAnimationComponent::GetHierarchyCenterOfMassWorld(FVector& OutWorldCOM) const
{
	// Prefer the physics body COM when available (subclass override)
	if (GetPhysicsBodyCenterOfMassWorld(OutWorldCOM))
	{
		return true;
	}
	float TotalMass = 0.f;
	ComputeMassesAndCOMs(StaticMeshComponents, OutWorldCOM, TotalMass, nullptr);
	return StaticMeshComponents.Num() > 0;
}

TArray<FStaticMeshMassAndCOM> UStaticMeshHierarchyAnimationComponent::GetPerMeshMassesAndCOMs() const
{
	TArray<FStaticMeshMassAndCOM> Result;
	FVector IgnoredCOM;
	float IgnoredMass;
	ComputeMassesAndCOMs(StaticMeshComponents, IgnoredCOM, IgnoredMass, &Result);
	return Result;
}


void UStaticMeshHierarchyAnimationComponent::BindStaticMeshComposerFinished()
{
	TArray<UActorComponent*> Components = GetOwner()->GetComponentsByInterface(UStaticMeshComposingInterface::StaticClass());

	if (Components.Num() == 0)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
			{
				// Retry seeking camera after a short delay
				UE_LOGFMT(Log_StaticMeshAnimator, Verbose, "{Time}: {Line} {Class}: No components implementing UStaticMeshComposingInterface found, retrying...",
					("Class", GET_CLASSNAME_WITH_FUNCTION),
					("Line", GET_LINE_NUMBER),
					("Time", GET_CURRENT_TIME));
				BindStaticMeshComposerFinished();
			});
		return;
	}

	IStaticMeshComposingInterface* Interface = Components.Num() > 0 ? Cast<IStaticMeshComposingInterface>(Components[0]) : nullptr;

	if (!Interface)
	{
		UE_LOGFMT(Log_StaticMeshAnimator, Warning, "{Time}: {Line} {Class}: Owner does not implement IStaticMeshComposingInterface",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	FNotifyMeshSetupComplete& PossessEvent = Interface->GetOnNotifyMeshSetupComplete();
	if (!PossessEvent.IsAlreadyBound(this, &UStaticMeshHierarchyAnimationComponent::OnNotifyMeshSetupComplete))
	{		
		PossessEvent.AddDynamic(this, &UStaticMeshHierarchyAnimationComponent::OnNotifyMeshSetupComplete);
	}

	UE_LOGFMT(Log_StaticMeshAnimator, Verbose, "{Time}: {Line} {Class}: StaticMeshComposingFinished delegate bound to OnNotifyMeshSetupComplete",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME));
}

void UStaticMeshHierarchyAnimationComponent::OnNotifyMeshSetupComplete(FMeshComponentCollection ComponentCollection)
{
	if(ComponentCollection.MeshComponents.Num() == 0)
	{
		UE_LOGFMT(Log_StaticMeshAnimator, Warning, "{Time}: {Line} {Class}: No static mesh components found in OnNotifyMeshSetupComplete",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	StaticMeshComponents = ComponentCollection.MeshComponents;

	UE_LOGFMT(Log_StaticMeshAnimator, Verbose, "{Time}: {Line} {Class}: OnNotifyMeshSetupComplete called with {Count} static mesh components",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Count", StaticMeshComponents.Num()));

	bool bSetupComplete = true;

	for (auto Component : StaticMeshComponents)
	{
		if (!Component.Value)
		{
			UE_LOGFMT(Log_StaticMeshAnimator, Warning, "{Time}: {Line} {Class}: Static mesh component {Name} is null",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("Name", Component.Key.ToString()));
			bSetupComplete = false;
			continue;
		}
		UE_LOGFMT(Log_StaticMeshAnimator, VeryVerbose, "{Time}: {Line} {Class}: Static mesh component {Name} setup complete",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Name", Component.Key.ToString()));
	}

	if (bSetupComplete)
	{
		UE_LOGFMT(Log_StaticMeshAnimator, Log, "{Time}: {Line} {Class}: Static mesh components setup complete with {Count} components",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Count", StaticMeshComponents.Num()));
		bIsStaticMeshCollectionAvailable = true;
	}
	else
	{
		UE_LOGFMT(Log_StaticMeshAnimator, Warning, "{Time}: {Line} {Class}: Static mesh components setup incomplete, some components are null",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
	}
}

TArray<FInformationSet> UStaticMeshHierarchyAnimationComponent::GetSettingsVariableSets_Implementation() const
{
	TArray<FInformationSet> InformationSets = TArray<FInformationSet>();

	FInformationSet StaticMeshCollectionInfoSet = UCoreInformationFunctionLibrary::GetInformationSet(
		"Static Mesh Collection",
		"",
		""
	);

	UCoreInformationFunctionLibrary::AddInformationValue(
		StaticMeshCollectionInfoSet,
		"StaticMeshCollection Available",
		bIsStaticMeshCollectionAvailable ? "True" : "False",
		EValueType::VT_BOOL
	);

	if(bIsStaticMeshCollectionAvailable)
	{
		UCoreInformationFunctionLibrary::AddInformationValue(
			StaticMeshCollectionInfoSet,
			"StaticMeshComponents Count",
			FString::FromInt(StaticMeshComponents.Num()),
			EValueType::VT_INT
		);
		UCoreInformationFunctionLibrary::AddInformationValue(
			StaticMeshCollectionInfoSet,
			"Components:",
			"",
			EValueType::VT_EMPTY
		);

		for (const auto& Component : StaticMeshComponents)
		{
			if (Component.Value)
			{
				UCoreInformationFunctionLibrary::AddInformationValue(
					StaticMeshCollectionInfoSet,
					"",
					Component.Key.ToString(),
					EValueType::VT_STRING
				);
			}
			else
			{
				UCoreInformationFunctionLibrary::AddInformationValue(
					StaticMeshCollectionInfoSet,
					"",
					"Null",
					EValueType::VT_STRING
				);
			}
		}
	}

	InformationSets.Add(StaticMeshCollectionInfoSet);

	InformationSets.Append(Super::GetSettingsVariableSets_Implementation());

	return InformationSets;
}