// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/InteractCollisionComponent.h"

#include "Blueprint/UserWidget.h"

#include "Components/BoxComponent.h"
#include "Components/Image.h"

#include "Enums/SenseType.h"

#include "Interfaces/ShouldAddGameFeatureInterface.h"
#include "Interfaces/HumanoidSenseInterface.h"
#include "Interfaces/SceneComponentCatchInterface.h"
#include "Interfaces/RegisterComponentInterface.h"

#include "Logging/GF_Interact_Logging.h"
#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"

#include "GameFeature_Interact_Settings.h"

UInteractCollisionComponent::UInteractCollisionComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;

	InteractableComponents = TArray<UBoxComponent*>();
#if WITH_EDITORONLY_DATA
	bDebugMode = true; // Default to false, can be set in the editor
#endif

	InteractableActors = TArray<AActor*>();
	CurrentInteractingActor = nullptr;

	bComponentHasComponentAssetToLoad = true;
	
	bHasLimit = false;
	Limit = 1;

	GameFeatureName = FString("Interact");
}

void UInteractCollisionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (IsGameFeatureIgnored())
	{
		UE_LOGFMT(Log_Interact, Verbose, "{Time}: {Line} {Class}: Game feature is ignored, skipping BeginPlay on Actor: {ActorName}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("ActorName", GetOwner() ? GetOwner()->GetName() : TEXT("null")));
		return;
	}

	UE_LOGFMT(Log_Interact, Verbose, "{Time}: {Line} {Class}: Interact Collision Component has started on Actor: {ActorName}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("ActorName", GetOwner()->GetName()));
}

void UInteractCollisionComponent::ProcessPossess()
{
	Super::ProcessPossess();

	if (GetWorld()->GetFirstPlayerController() == GetOwner()->GetInstigatorController())
	{
		RemoveWidget();
		UE_LOGFMT(Log_Interact, Verbose, "{Time}: {Line} {Class}: ProcessPossess: removed interact widget (owner possessed by first player).",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
	}
}

void UInteractCollisionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	RemoveWidget();
	UE_LOGFMT(Log_Interact, Verbose, "{Time}: {Line} {Class}: EndPlay (Reason={Reason}), interact widget removed.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Reason", static_cast<int32>(EndPlayReason)));
}

const bool UInteractCollisionComponent::CheckLimit() const
{
	if (bHasLimit)
	{
		if (Limit > 0)
		{
			return true;
		}
		else
		{
			UE_LOGFMT(Log_Interact, VeryVerbose, "{Time}: {Line} {Class}: Cannot interact again with this actor. Limit reached.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));
			return false;
		}
	}
	return true;
}

void UInteractCollisionComponent::UnlockTemporaryRegisterLock()
{
	bTemporaryTickLockForRegister = false;
	UE_LOGFMT(Log_Interact, VeryVerbose, "{Time}: {Line} {Class}: Temporary tick lock for register is released",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME));
}

#pragma region Debug Editor Tick
#if WITH_EDITOR
void UInteractCollisionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bDebugCollisionBoxComponent)
	{
		if(!bHasDebuggedCollisionBoxComponent)
		{
			UE_LOGFMT(Log_Interact_Debug, VeryVerbose, "{Time}: {Line} {Class}: Showing debug collision box components",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));
			for (int32 i = 0; i < InteractableComponents.Num(); i++)
			{
				if (UBoxComponent* Component = InteractableComponents[i])
				{
					Component->SetVisibility(true);
					Component->SetHiddenInGame(false);
				}
			}
			bHasDebuggedCollisionBoxComponent = true;
		}
	}
	else
	{
		if (bHasDebuggedCollisionBoxComponent)
		{
			UE_LOGFMT(Log_Interact_Debug, VeryVerbose, "{Time}: {Line} {Class}: Hiding debug collision box components",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));
			for (int32 i = 0; i < InteractableComponents.Num(); i++)
			{
				if (UBoxComponent* Component = InteractableComponents[i])
				{
					Component->SetVisibility(false);
					Component->SetHiddenInGame(true);
				}
			}
		}
	}

	if (bDebugMode)
	{
		for (int32 i = 0; i < InteractableComponents.Num(); i++)
		{
			if (UBoxComponent* Component = InteractableComponents[i])
			{
				FVector Extents = Component->GetScaledBoxExtent();
				//Component->GetComponentBou

				DrawDebugBox(
					GetWorld(),
					Component->GetComponentLocation(),
					Extents,
					Component->GetComponentQuat(),
					FColor::Green,
					false,
					DeltaTime,
					0,
					1.f
				);
			}
			else
			{
				UE_LOGFMT(Log_Interact_Debug, Error, "{Time}: {Line} {Class}: Interactable collision component at index {Index} is null",
					("Class", GET_CLASSNAME_WITH_FUNCTION),
					("Line", GET_LINE_NUMBER),
					("Time", GET_CURRENT_TIME),
					("Index", i));
			}
		}
	}
}
#endif
#pragma endregion