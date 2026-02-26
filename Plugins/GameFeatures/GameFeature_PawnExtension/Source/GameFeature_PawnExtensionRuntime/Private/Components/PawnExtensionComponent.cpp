// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#include "Components/PawnExtensionComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Interfaces/AbilitySystemComponentInterface.h"
#include "GameplayTags_Core.h"
#include "Interfaces/AbilitySystemTagRelationshipReceiverInterface.h"
#include "Interfaces/PawnDataTagRelationshipProviderInterface.h"
#include "Components/GameFrameworkComponentDelegates.h"
#include "Components/GameFrameworkComponentManager.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PawnExtensionComponent)

class FLifetimeProperty;
class UActorComponent;

const FName UPawnExtensionComponent::NAME_ActorFeatureName("PawnExtension");

UPawnExtensionComponent::UPawnExtensionComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

	PawnData = nullptr;
	AbilitySystemComponent = nullptr;
}

void UPawnExtensionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPawnExtensionComponent, PawnData);
}

void UPawnExtensionComponent::OnRegister()
{
	Super::OnRegister();

	const APawn* Pawn = GetPawn<APawn>();
	ensureAlwaysMsgf((Pawn != nullptr), TEXT("PawnExtensionComponent on [%s] can only be added to Pawn actors."), *GetNameSafe(GetOwner()));

	TArray<UActorComponent*> PawnExtensionComponents;
	Pawn->GetComponents(UPawnExtensionComponent::StaticClass(), PawnExtensionComponents);
	ensureAlwaysMsgf((PawnExtensionComponents.Num() == 1), TEXT("Only one PawnExtensionComponent should exist on [%s]."), *GetNameSafe(GetOwner()));

	// Register with the init state system early, this will only work if this is a game world
	RegisterInitStateFeature();
}

void UPawnExtensionComponent::BeginPlay()
{
	Super::BeginPlay();

	// Listen for changes to all features
	BindOnActorInitStateChanged(NAME_None, FGameplayTag(), false);
	
	// Notifies state manager that we have spawned, then try rest of default initialization
	ensure(TryToChangeInitState(GameplayTags_Core::InitState_Spawned));
	CheckDefaultInitialization();
}

void UPawnExtensionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UninitializeAbilitySystem();
	UnregisterInitStateFeature();

	Super::EndPlay(EndPlayReason);
}

void UPawnExtensionComponent::SetPawnData(const UObject* InPawnData)
{
	check(InPawnData);

	APawn* Pawn = GetPawnChecked<APawn>();

	if (Pawn->GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (PawnData)
	{
		UE_LOG(LogTemp, Error, TEXT("Trying to set PawnData [%s] on pawn [%s] that already has valid PawnData [%s]."), *GetNameSafe(InPawnData), *GetNameSafe(Pawn), *GetNameSafe(PawnData));
		return;
	}

	PawnData = InPawnData;

	Pawn->ForceNetUpdate();

	CheckDefaultInitialization();
}

void UPawnExtensionComponent::OnRep_PawnData()
{
	CheckDefaultInitialization();
}

void UPawnExtensionComponent::InitializeAbilitySystem_Implementation(UAbilitySystemComponent* ASC, UObject* Owner)
{
	if (ASC && Owner)
	{
		InitializeAbilitySystem(TScriptInterface<UAbilitySystemComponentInterface>(ASC), Cast<AActor>(Owner));
	}
}

void UPawnExtensionComponent::InitializeAbilitySystem(TScriptInterface<UAbilitySystemComponentInterface> InASC, AActor* InOwnerActor)
{
	UAbilitySystemComponent* ASC = Cast<UAbilitySystemComponent>(InASC.GetObject());
	check(ASC);
	check(InOwnerActor);

	if (AbilitySystemComponent.GetObject() == ASC)
	{
		// The ability system component hasn't changed.
		return;
	}

	if (AbilitySystemComponent.GetObject())
	{
		// Clean up the old ability system component.
		UninitializeAbilitySystem();
	}

	APawn* Pawn = GetPawnChecked<APawn>();
	AActor* ExistingAvatar = ASC->GetAvatarActor();

	UE_LOG(LogTemp, Verbose, TEXT("Setting up ASC [%s] on pawn [%s] owner [%s], existing [%s] "), *GetNameSafe(ASC), *GetNameSafe(Pawn), *GetNameSafe(InOwnerActor), *GetNameSafe(ExistingAvatar));

	if ((ExistingAvatar != nullptr) && (ExistingAvatar != Pawn))
	{
		UE_LOG(LogTemp, Log, TEXT("Existing avatar (authority=%d)"), ExistingAvatar->HasAuthority() ? 1 : 0);

		// There is already a pawn acting as the ASC's avatar, so we need to kick it out
		// This can happen on clients if they're lagged: their new pawn is spawned + possessed before the dead one is removed
		ensure(!ExistingAvatar->HasAuthority());

		if (UPawnExtensionComponent* OtherExtensionComponent = FindPawnExtensionComponent(ExistingAvatar))
		{
			OtherExtensionComponent->UninitializeAbilitySystem();
		}
	}

	AbilitySystemComponent = InASC;
	ASC->InitAbilityActorInfo(InOwnerActor, Pawn);

	if (PawnData)
	{
		const IPawnDataTagRelationshipProviderInterface* Provider = Cast<IPawnDataTagRelationshipProviderInterface>(PawnData);
		IAbilitySystemTagRelationshipReceiverInterface* Receiver = Cast<IAbilitySystemTagRelationshipReceiverInterface>(ASC);
		if (Provider && Receiver)
		{
			if (UObject* Mapping = Provider->GetTagRelationshipMapping())
			{
				Receiver->SetTagRelationshipMapping(Mapping);
			}
		}
	}

	OnAbilitySystemInitialized.Broadcast();
}

void UPawnExtensionComponent::UninitializeAbilitySystem()
{
	UAbilitySystemComponent* ASC = Cast<UAbilitySystemComponent>(AbilitySystemComponent.GetObject());
	if (!ASC)
	{
		return;
	}

	// Uninitialize the ASC if we're still the avatar actor (otherwise another pawn already did it when they became the avatar actor)
	if (ASC->GetAvatarActor() == GetOwner())
	{
		FGameplayTagContainer AbilityTypesToIgnore;
		AbilityTypesToIgnore.AddTag(GameplayTags_Core::Ability_Behavior_SurvivesDeath);

		ASC->CancelAbilities(nullptr, &AbilityTypesToIgnore);
		// ClearAbilityInput: Lyra/Core extended ASC only; use custom ASC override if needed
		ASC->RemoveAllGameplayCues();

		if (ASC->GetOwnerActor() != nullptr)
		{
			ASC->SetAvatarActor(nullptr);
		}
		else
		{
			// If the ASC doesn't have a valid owner, we need to clear *all* actor info, not just the avatar pairing
			ASC->ClearActorInfo();
		}

		OnAbilitySystemUninitialized.Broadcast();
	}

	AbilitySystemComponent = nullptr;
}

void UPawnExtensionComponent::HandleControllerChanged()
{
	UAbilitySystemComponent* ASC = Cast<UAbilitySystemComponent>(AbilitySystemComponent.GetObject());
	if (ASC && (ASC->GetAvatarActor() == GetPawnChecked<APawn>()))
	{
		ensure(ASC->AbilityActorInfo->OwnerActor == ASC->GetOwnerActor());
		if (ASC->GetOwnerActor() == nullptr)
		{
			UninitializeAbilitySystem();
		}
		else
		{
			ASC->RefreshAbilityActorInfo();
		}
	}

	CheckDefaultInitialization();
}

void UPawnExtensionComponent::HandlePlayerStateReplicated()
{
	CheckDefaultInitialization();
}

//~IPawnExtensionComponentInterface (Elemental)
UAbilitySystemComponent* UPawnExtensionComponent::GetAbilitySystemComponent_Implementation() const
{
	return Cast<UAbilitySystemComponent>(AbilitySystemComponent.GetObject());
}

void UPawnExtensionComponent::SetPawnData_Implementation(UObject* InPawnData)
{
	SetPawnData(InPawnData);
}

void UPawnExtensionComponent::UninitializeAbilitySystem_Implementation()
{
	UninitializeAbilitySystem();
}

void UPawnExtensionComponent::HandleControllerChanged_Implementation()
{
	HandleControllerChanged();
}

void UPawnExtensionComponent::HandlePlayerStateReplicated_Implementation()
{
	HandlePlayerStateReplicated();
}

void UPawnExtensionComponent::SetupPlayerInputComponent()
{
	CheckDefaultInitialization();
}

void UPawnExtensionComponent::CheckDefaultInitialization()
{
	// Before checking our progress, try progressing any other features we might depend on
	CheckDefaultInitializationForImplementers();

	static const TArray<FGameplayTag> StateChain = { 
		GameplayTags_Core::InitState_Spawned, 
		GameplayTags_Core::InitState_DataAvailable, 
		GameplayTags_Core::InitState_DataInitialized, 
		GameplayTags_Core::InitState_GameplayReady 
	};

	// This will try to progress from spawned (which is only set in BeginPlay) through the data initialization stages until it gets to gameplay ready
	ContinueInitStateChain(StateChain);
}

bool UPawnExtensionComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	APawn* Pawn = GetPawn<APawn>();
	if (!CurrentState.IsValid() && DesiredState == GameplayTags_Core::InitState_Spawned)
	{
		// As long as we are on a valid pawn, we count as spawned
		if (Pawn)
		{
			return true;
		}
	}
	if (CurrentState == GameplayTags_Core::InitState_Spawned && DesiredState == GameplayTags_Core::InitState_DataAvailable)
	{
		// Pawn data is required.
		if (!PawnData)
		{
			return false;
		}

		const bool bHasAuthority = Pawn->HasAuthority();
		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();

		if (bHasAuthority || bIsLocallyControlled)
		{
			// Check for being possessed by a controller.
			if (!GetController<AController>())
			{
				return false;
			}
		}

		return true;
	}
	else if (CurrentState == GameplayTags_Core::InitState_DataAvailable && DesiredState == GameplayTags_Core::InitState_DataInitialized)
	{
		// Transition to initialize if all features have their data available
		return Manager->HaveAllFeaturesReachedInitState(Pawn, GameplayTags_Core::InitState_DataAvailable);
	}
	else if (CurrentState == GameplayTags_Core::InitState_DataInitialized && DesiredState == GameplayTags_Core::InitState_GameplayReady)
	{
		return true;
	}

	return false;
}

void UPawnExtensionComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	if (DesiredState == GameplayTags_Core::InitState_DataInitialized)
	{
		// This is currently all handled by other components listening to this state change
	}
}

void UPawnExtensionComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	// If another feature is now in DataAvailable, see if we should transition to DataInitialized
	if (Params.FeatureName != NAME_ActorFeatureName)
	{
		if (Params.FeatureState == GameplayTags_Core::InitState_DataAvailable)
		{
			CheckDefaultInitialization();
		}
	}
}

void UPawnExtensionComponent::OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate Delegate)
{
	if (!OnAbilitySystemInitialized.IsBoundToObject(Delegate.GetUObject()))
	{
		OnAbilitySystemInitialized.Add(Delegate);
	}

	if (AbilitySystemComponent)
	{
		Delegate.Execute();
	}
}

void UPawnExtensionComponent::OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate Delegate)
{
	if (!OnAbilitySystemUninitialized.IsBoundToObject(Delegate.GetUObject()))
	{
		OnAbilitySystemUninitialized.Add(Delegate);
	}
}
