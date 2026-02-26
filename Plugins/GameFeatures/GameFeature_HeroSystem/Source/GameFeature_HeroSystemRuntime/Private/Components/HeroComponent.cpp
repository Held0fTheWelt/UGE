// Copyright Epic Games, Inc. All Rights Reserved.

#include "Components/HeroComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Components/GameFrameworkComponentDelegates.h"
#include "Interfaces/AbilitySystemInputInterface.h"
#include "Components/GameFrameworkComponentManager.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/PawnExtensionComponentInterface.h"
#include "Subsystems/ElementalInitStateTagSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HeroComponent)

const FName UHeroComponent::NAME_BindInputsNow("BindInputsNow");
const FName UHeroComponent::NAME_ActorFeatureName("Hero");

UHeroComponent::UHeroComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilityCameraMode = nullptr;
	bReadyToBindInputs = false;
}

void UHeroComponent::OnRegister()
{
	Super::OnRegister();

	if (!GetPawn<APawn>())
	{
		UE_LOG(LogTemp, Error, TEXT("[UHeroComponent::OnRegister] This component has been added to a blueprint whose base class is not a Pawn. To use this component, it MUST be placed on a Pawn Blueprint."));
	}
	else
	{
		RegisterInitStateFeature();
	}
}

bool UHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	APawn* Pawn = GetPawn<APawn>();
	UWorld* World = GetWorld();
	UElementalInitStateTagSubsystem* InitStateTags = World ? World->GetSubsystem<UElementalInitStateTagSubsystem>() : nullptr;
	if (!InitStateTags)
	{
		return false;
	}

	const FGameplayTag InitState_Spawned = InitStateTags->GetInitStateSpawned();
	const FGameplayTag InitState_DataAvailable = InitStateTags->GetInitStateDataAvailable();
	const FGameplayTag InitState_DataInitialized = InitStateTags->GetInitStateDataInitialized();
	const FGameplayTag InitState_GameplayReady = InitStateTags->GetInitStateGameplayReady();
	const FName PawnExtensionFeatureName = InitStateTags->GetPawnExtensionFeatureName();

	if (!CurrentState.IsValid() && DesiredState == InitState_Spawned)
	{
		if (Pawn)
		{
			return true;
		}
	}
	else if (CurrentState == InitState_Spawned && DesiredState == InitState_DataAvailable)
	{
		APlayerState* PlayerState = GetPlayerState<APlayerState>();
		if (!PlayerState || !Cast<IAbilitySystemInterface>(PlayerState))
		{
			return false;
		}

		if (Pawn->GetLocalRole() != ROLE_SimulatedProxy)
		{
			AController* Controller = GetController<AController>();
			const bool bHasControllerPairedWithPS = (Controller != nullptr) &&
				(Controller->PlayerState != nullptr) &&
				(Controller->PlayerState->GetOwner() == Controller);
			if (!bHasControllerPairedWithPS)
			{
				return false;
			}
		}

		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const bool bIsBot = Pawn->IsBotControlled();
		if (bIsLocallyControlled && !bIsBot)
		{
			APlayerController* PC = GetController<APlayerController>();
			if (!Pawn->InputComponent || !PC || !PC->GetLocalPlayer())
			{
				return false;
			}
		}

		return true;
	}
	else if (CurrentState == InitState_DataAvailable && DesiredState == InitState_DataInitialized)
	{
		APlayerState* PlayerState = GetPlayerState<APlayerState>();
		return PlayerState && Manager->HasFeatureReachedInitState(Pawn, PawnExtensionFeatureName, InitState_DataInitialized);
	}
	else if (CurrentState == InitState_DataInitialized && DesiredState == InitState_GameplayReady)
	{
		return true;
	}

	return false;
}

void UHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	UWorld* World = GetWorld();
	UElementalInitStateTagSubsystem* InitStateTags = World ? World->GetSubsystem<UElementalInitStateTagSubsystem>() : nullptr;
	if (!InitStateTags)
	{
		return;
	}
	const FGameplayTag InitState_DataAvailable = InitStateTags->GetInitStateDataAvailable();
	const FGameplayTag InitState_DataInitialized = InitStateTags->GetInitStateDataInitialized();

	if (CurrentState == InitState_DataAvailable && DesiredState == InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		APlayerState* PlayerState = GetPlayerState<APlayerState>();
		if (!ensure(Pawn && PlayerState))
		{
			return;
		}

		IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(PlayerState);
		UAbilitySystemComponent* ASC = ASI ? ASI->GetAbilitySystemComponent() : nullptr;

		UActorComponent* PawnExtComp = Pawn->FindComponentByInterface(UPawnExtensionComponentInterface::StaticClass());
		if (PawnExtComp && ASC)
		{
			IPawnExtensionComponentInterface::Execute_InitializeAbilitySystem(PawnExtComp, ASC, PlayerState);
		}

		if (APlayerController* PC = GetController<APlayerController>())
		{
			if (Pawn->InputComponent != nullptr)
			{
				InitializePlayerInput(Pawn->InputComponent);
			}
		}
	}
}

void UHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	UWorld* World = GetWorld();
	UElementalInitStateTagSubsystem* InitStateTags = World ? World->GetSubsystem<UElementalInitStateTagSubsystem>() : nullptr;
	if (!InitStateTags || Params.FeatureName != InitStateTags->GetPawnExtensionFeatureName())
	{
		return;
	}
	if (Params.FeatureState == InitStateTags->GetInitStateDataInitialized())
	{
		CheckDefaultInitialization();
	}
}

void UHeroComponent::CheckDefaultInitialization()
{
	UWorld* World = GetWorld();
	UElementalInitStateTagSubsystem* InitStateTags = World ? World->GetSubsystem<UElementalInitStateTagSubsystem>() : nullptr;
	if (!InitStateTags)
	{
		return;
	}

	TArray<FGameplayTag> StateChain;
	StateChain.Add(InitStateTags->GetInitStateSpawned());
	StateChain.Add(InitStateTags->GetInitStateDataAvailable());
	StateChain.Add(InitStateTags->GetInitStateDataInitialized());
	StateChain.Add(InitStateTags->GetInitStateGameplayReady());
	ContinueInitStateChain(StateChain);
}

void UHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	UElementalInitStateTagSubsystem* InitStateTags = World ? World->GetSubsystem<UElementalInitStateTagSubsystem>() : nullptr;
	if (InitStateTags)
	{
		BindOnActorInitStateChanged(InitStateTags->GetPawnExtensionFeatureName(), FGameplayTag(), false);
	}

	if (InitStateTags)
	{
		ensure(TryToChangeInitState(InitStateTags->GetInitStateSpawned()));
	}
	CheckDefaultInitialization();
}

void UHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();
	Super::EndPlay(EndPlayReason);
}

void UHeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	if (ensure(!bReadyToBindInputs))
	{
		bReadyToBindInputs = true;
	}

	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APlayerController*>(PC), NAME_BindInputsNow);
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APawn*>(Pawn), NAME_BindInputsNow);
}

void UHeroComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	UActorComponent* PawnExtComp = Pawn->FindComponentByInterface(UPawnExtensionComponentInterface::StaticClass());
	if (PawnExtComp)
	{
		UAbilitySystemComponent* ASC = IPawnExtensionComponentInterface::Execute_GetAbilitySystemComponent(PawnExtComp);
		if (IAbilitySystemInputInterface* InputInterface = Cast<IAbilitySystemInputInterface>(ASC))
		{
			InputInterface->AbilityInputTagPressed(InputTag);
		}
	}
}

void UHeroComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	UActorComponent* PawnExtComp = Pawn->FindComponentByInterface(UPawnExtensionComponentInterface::StaticClass());
	if (PawnExtComp)
	{
		UAbilitySystemComponent* ASC = IPawnExtensionComponentInterface::Execute_GetAbilitySystemComponent(PawnExtComp);
		if (IAbilitySystemInputInterface* InputInterface = Cast<IAbilitySystemInputInterface>(ASC))
		{
			InputInterface->AbilityInputTagReleased(InputTag);
		}
	}
}

void UHeroComponent::SetAbilityCameraMode(TSubclassOf<UObject> CameraMode, const FGameplayAbilitySpecHandle& OwningSpecHandle)
{
	AbilityCameraMode = CameraMode;
	AbilityCameraModeOwningSpecHandle = OwningSpecHandle;
}

void UHeroComponent::ClearAbilityCameraMode(const FGameplayAbilitySpecHandle& OwningSpecHandle)
{
	if (AbilityCameraModeOwningSpecHandle == OwningSpecHandle)
	{
		AbilityCameraMode = nullptr;
		AbilityCameraModeOwningSpecHandle = FGameplayAbilitySpecHandle();
	}
}

bool UHeroComponent::IsReadyToBindInputs() const
{
	return bReadyToBindInputs;
}
