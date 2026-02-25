// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#include "Characters/CoreCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Elemental_Interfaces/Public/Interfaces/PawnExtensionComponentInterface.h"
#include "AbilitySystemComponent.h"
#include "GameplayTags_Core.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CoreCharacter)

static UActorComponent* FindPawnExtensionComponent(AActor* Actor)
{
	if (!Actor) return nullptr;
	for (UActorComponent* Comp : Actor->GetComponents())
	{
		if (Comp && Comp->GetClass()->ImplementsInterface(UPawnExtensionComponentInterface::StaticClass()))
		{
			return Comp;
		}
	}
	return nullptr;
}

//////////////////////////////////////////////////////////////////////
// FCoreSharedRepMovement

FCoreSharedRepMovement::FCoreSharedRepMovement()
{
	RepMovement.LocationQuantizationLevel = EVectorQuantization::RoundTwoDecimals;
}

bool FCoreSharedRepMovement::FillForCharacter(ACharacter* Character)
{
	if (USceneComponent* PawnRootComponent = Character->GetRootComponent())
	{
		UCharacterMovementComponent* CharacterMovement = Character->GetCharacterMovement();

		RepMovement.Location = FRepMovement::RebaseOntoZeroOrigin(PawnRootComponent->GetComponentLocation(), Character);
		RepMovement.Rotation = PawnRootComponent->GetComponentRotation();
		RepMovement.LinearVelocity = CharacterMovement->Velocity;
		RepMovementMode = CharacterMovement->PackNetworkMovementMode();
		bProxyIsJumpForceApplied = Character->GetProxyIsJumpForceApplied() || (Character->JumpForceTimeRemaining > 0.0f);
		bIsCrouched = Character->IsCrouched();

		// Timestamp is sent as zero if unused
		if ((CharacterMovement->NetworkSmoothingMode == ENetworkSmoothingMode::Linear) || CharacterMovement->bNetworkAlwaysReplicateTransformUpdateTimestamp)
		{
			RepTimeStamp = CharacterMovement->GetServerLastTransformUpdateTimeStamp();
		}
		else
		{
			RepTimeStamp = 0.f;
		}

		return true;
	}
	return false;
}

bool FCoreSharedRepMovement::Equals(const FCoreSharedRepMovement& Other, ACharacter* Character) const
{
	if (RepMovement.Location != Other.RepMovement.Location)
	{
		return false;
	}

	if (RepMovement.Rotation != Other.RepMovement.Rotation)
	{
		return false;
	}

	if (RepMovement.LinearVelocity != Other.RepMovement.LinearVelocity)
	{
		return false;
	}

	if (RepMovementMode != Other.RepMovementMode)
	{
		return false;
	}

	if (bProxyIsJumpForceApplied != Other.bProxyIsJumpForceApplied)
	{
		return false;
	}

	if (bIsCrouched != Other.bIsCrouched)
	{
		return false;
	}

	return true;
}

bool FCoreSharedRepMovement::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	bOutSuccess = true;
	RepMovement.NetSerialize(Ar, Map, bOutSuccess);
	Ar << RepMovementMode;
	Ar << bProxyIsJumpForceApplied;
	Ar << bIsCrouched;

	// Timestamp, if non-zero.
	uint8 bHasTimeStamp = (RepTimeStamp != 0.f);
	Ar.SerializeBits(&bHasTimeStamp, 1);
	if (bHasTimeStamp)
	{
		Ar << RepTimeStamp;
	}
	else
	{
		RepTimeStamp = 0.f;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
// ACoreCharacter

ACoreCharacter::ACoreCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, MyTeamID(FGenericTeamId::NoTeam)
{
	// Character-specific setup can go here if needed
	// But remember: Movement, Camera, GAS come via GameFeatures!
	
	// Avoid ticking characters if possible
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	SetNetCullDistanceSquared(900000000.0f);
}

void ACoreCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ThisClass, ReplicatedAcceleration, COND_SimulatedOnly);
	DOREPLIFETIME_CONDITION(ThisClass, MyTeamID, COND_InitialOnly);
}

void ACoreCharacter::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);

	if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		// Compress Acceleration: XY components as direction + magnitude, Z component as direct value
		const double MaxAccel = MovementComponent->MaxAcceleration;
		const FVector CurrentAccel = MovementComponent->GetCurrentAcceleration();
		double AccelXYRadians, AccelXYMagnitude;
		FMath::CartesianToPolar(CurrentAccel.X, CurrentAccel.Y, AccelXYMagnitude, AccelXYRadians);

		ReplicatedAcceleration.AccelXYRadians   = FMath::FloorToInt((AccelXYRadians / TWO_PI) * 255.0);     // [0, 2PI] -> [0, 255]
		ReplicatedAcceleration.AccelXYMagnitude = FMath::FloorToInt((AccelXYMagnitude / MaxAccel) * 255.0);	// [0, MaxAccel] -> [0, 255]
		ReplicatedAcceleration.AccelZ           = FMath::FloorToInt((CurrentAccel.Z / MaxAccel) * 127.0);   // [-MaxAccel, MaxAccel] -> [-127, 127]
	}
}

void ACoreCharacter::SetReplicatedAcceleration(const FVector& InAcceleration)
{
	// UCharacterMovementComponent::Acceleration is protected in UE5; use a derived movement component with a public setter if replication is needed.
	(void)InAcceleration;
}

bool ACoreCharacter::UpdateSharedReplication()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		FCoreSharedRepMovement SharedMovement;
		if (SharedMovement.FillForCharacter(this))
		{
			// Only call FastSharedReplication if data has changed since the last frame.
			// Skipping this call will cause replication to reuse the same bunch that we previously
			// produced, but not send it to clients that already received. (But a new client who has not received
			// it, will get it this frame)
			if (!SharedMovement.Equals(LastSharedReplication, this))
			{
				LastSharedReplication = SharedMovement;
				SetReplicatedMovementMode(SharedMovement.RepMovementMode);

				FastSharedReplication(SharedMovement);
			}
			return true;
		}
	}

	// We cannot fastrep right now. Don't send anything.
	return false;
}

void ACoreCharacter::FastSharedReplication_Implementation(const FCoreSharedRepMovement& SharedRepMovement)
{
	if (GetWorld()->IsPlayingReplay())
	{
		return;
	}

	// Timestamp is checked to reject old moves.
	if (GetLocalRole() == ROLE_SimulatedProxy)
	{
		// Timestamp
		SetReplicatedServerLastTransformUpdateTimeStamp(SharedRepMovement.RepTimeStamp);

		// Movement mode
		if (GetReplicatedMovementMode() != SharedRepMovement.RepMovementMode)
		{
			SetReplicatedMovementMode(SharedRepMovement.RepMovementMode);
			GetCharacterMovement()->bNetworkMovementModeChanged = true;
			GetCharacterMovement()->bNetworkUpdateReceived = true;
		}

		// Location, Rotation, Velocity, etc.
		FRepMovement& MutableRepMovement = GetReplicatedMovement_Mutable();
		MutableRepMovement = SharedRepMovement.RepMovement;

		// This also sets LastRepMovement
		OnRep_ReplicatedMovement();

		// Jump force
		SetProxyIsJumpForceApplied(SharedRepMovement.bProxyIsJumpForceApplied);

		// Crouch
		if (IsCrouched() != SharedRepMovement.bIsCrouched)
		{
			SetIsCrouched(SharedRepMovement.bIsCrouched);
			OnRep_IsCrouched();
		}
	}
}

void ACoreCharacter::OnRep_ReplicatedAcceleration()
{
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		const double MaxAccel = MoveComp->MaxAcceleration;
		const double AccelXYMagnitude = double(ReplicatedAcceleration.AccelXYMagnitude) * MaxAccel / 255.0; // [0, 255] -> [0, MaxAccel]
		const double AccelXYRadians = double(ReplicatedAcceleration.AccelXYRadians) * TWO_PI / 255.0;     // [0, 255] -> [0, 2PI]

		FVector UnpackedAcceleration(FVector::ZeroVector);
		FMath::PolarToCartesian(AccelXYMagnitude, AccelXYRadians, UnpackedAcceleration.X, UnpackedAcceleration.Y);
		UnpackedAcceleration.Z = double(ReplicatedAcceleration.AccelZ) * MaxAccel / 127.0; // [-127, 127] -> [-MaxAccel, MaxAccel]

		if (IReplicatedAccelerationReceiver* Receiver = Cast<IReplicatedAccelerationReceiver>(MoveComp))
		{
			Receiver->SetReplicatedAcceleration(UnpackedAcceleration);
		}
	}
}

//~IAbilitySystemInterface
UAbilitySystemComponent* ACoreCharacter::GetAbilitySystemComponent() const
{
	return GetCoreAbilitySystemComponent();
}

void ACoreCharacter::InitAbilityActorInfo(UObject* InOwnerActor, UObject* InAvatarActor)
{
	if (UAbilitySystemComponent* ASC = GetCoreAbilitySystemComponent())
	{
		ASC->InitAbilityActorInfo(Cast<AActor>(InOwnerActor), Cast<AActor>(InAvatarActor));
	}
}

void ACoreCharacter::ClearActorInfo()
{
	if (UAbilitySystemComponent* ASC = GetCoreAbilitySystemComponent())
	{
		ASC->ClearActorInfo();
	}
}
//~End of IAbilitySystemInterface

UAbilitySystemComponent* ACoreCharacter::GetCoreAbilitySystemComponent() const
{
	if (UActorComponent* PawnExtComp = FindPawnExtensionComponent(const_cast<ACoreCharacter*>(this)))
	{
		return IPawnExtensionComponentInterface::Execute_GetAbilitySystemComponent(PawnExtComp);
	}
	return nullptr;
}

//~IGameplayTagAssetInterface
void ACoreCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (const UAbilitySystemComponent* ASC = GetCoreAbilitySystemComponent())
	{
		ASC->GetOwnedGameplayTags(TagContainer);
	}
}

bool ACoreCharacter::HasMatchingGameplayTag(FGameplayTag TagToCheck) const
{
	if (const UAbilitySystemComponent* ASC = GetCoreAbilitySystemComponent())
	{
		return ASC->HasMatchingGameplayTag(TagToCheck);
	}
	return false;
}

bool ACoreCharacter::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	if (const UAbilitySystemComponent* ASC = GetCoreAbilitySystemComponent())
	{
		return ASC->HasAllMatchingGameplayTags(TagContainer);
	}
	return false;
}

bool ACoreCharacter::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	if (const UAbilitySystemComponent* ASC = GetCoreAbilitySystemComponent())
	{
		return ASC->HasAnyMatchingGameplayTags(TagContainer);
	}
	return false;
}
//~End of IGameplayTagAssetInterface

//~IDeathEventInterface
void ACoreCharacter::OnDeathStarted(AActor* OwningActor)
{
	DisableMovementAndCollision();
}

void ACoreCharacter::OnDeathFinished(AActor* OwningActor)
{
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::DestroyDueToDeath);
}
//~End of IDeathEventInterface

void ACoreCharacter::DisableMovementAndCollision()
{
	if (GetController())
	{
		GetController()->SetIgnoreMoveInput(true);
	}

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	if (CapsuleComp)
	{
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	}

	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->StopMovementImmediately();
		MoveComp->DisableMovement();
	}
}

void ACoreCharacter::DestroyDueToDeath()
{
	// Uninitialize the ASC if we're still the avatar actor
	if (UAbilitySystemComponent* ASC = GetCoreAbilitySystemComponent())
	{
		if (ASC->GetAvatarActor() == this)
		{
			if (UActorComponent* PawnExtComp = FindPawnExtensionComponent(this))
			{
				IPawnExtensionComponentInterface::Execute_UninitializeAbilitySystem(PawnExtComp);
			}
		}
	}

	if (GetLocalRole() == ROLE_Authority)
	{
		DetachFromControllerPendingDestroy();
		SetLifeSpan(0.1f);
	}

	SetActorHiddenInGame(true);
}

//~IMovementModeTagInterface
void ACoreCharacter::SetMovementModeTag(EMovementMode MovementMode, uint8 CustomMovementMode, bool bTagEnabled)
{
	if (UAbilitySystemComponent* ASC = GetCoreAbilitySystemComponent())
	{
		const FGameplayTag* MovementModeTag = nullptr;
		if (MovementMode == MOVE_Custom)
		{
			MovementModeTag = GameplayTags_Core::CustomMovementModeTagMap.Find(CustomMovementMode);
		}
		else
		{
			MovementModeTag = GameplayTags_Core::MovementModeTagMap.Find(MovementMode);
		}

		if (MovementModeTag && MovementModeTag->IsValid())
		{
			ASC->SetLooseGameplayTagCount(*MovementModeTag, (bTagEnabled ? 1 : 0));
		}
	}
}

void ACoreCharacter::InitializeMovementModeTags()
{
	if (UAbilitySystemComponent* ASC = GetCoreAbilitySystemComponent())
	{
		// Clear tags that may be lingering on the ability system from the previous pawn
		for (const TPair<uint8, FGameplayTag>& TagMapping : GameplayTags_Core::MovementModeTagMap)
		{
			if (TagMapping.Value.IsValid())
			{
				ASC->SetLooseGameplayTagCount(TagMapping.Value, 0);
			}
		}

		for (const TPair<uint8, FGameplayTag>& TagMapping : GameplayTags_Core::CustomMovementModeTagMap)
		{
			if (TagMapping.Value.IsValid())
			{
				ASC->SetLooseGameplayTagCount(TagMapping.Value, 0);
			}
		}

		// Set initial movement mode tag
		if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
		{
			SetMovementModeTag(MoveComp->MovementMode, MoveComp->CustomMovementMode, true);
		}
	}
}

void ACoreCharacter::SetCrouchingTagCount(int32 Count)
{
	if (UAbilitySystemComponent* ASC = GetCoreAbilitySystemComponent())
	{
		ASC->SetLooseGameplayTagCount(GameplayTags_Core::Status_Crouching, Count);
	}
}
//~End of IMovementModeTagInterface

//~ITeamAgentInterface
void ACoreCharacter::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	if (HasAuthority())
	{
		const FGenericTeamId OldTeamID = MyTeamID;
		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, MyTeamID, this);
		MyTeamID = NewTeamID;
		ITeamAgentInterface::ConditionalBroadcastTeamChanged(this, OldTeamID, NewTeamID);
	}
}

FGenericTeamId ACoreCharacter::GetGenericTeamId() const
{
	return MyTeamID;
}

FOnTeamIndexChangedDelegate* ACoreCharacter::GetOnTeamIndexChangedDelegate()
{
	return &OnTeamChangedDelegate;
}

float ACoreCharacter::GetGroundDistance() const
{
	if (const UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		if (MoveComp->CurrentFloor.bBlockingHit)
		{
			return MoveComp->CurrentFloor.FloorDist;
		}
	}
	return -1.0f;
}
//~End of ITeamAgentInterface

void ACoreCharacter::OnRep_MyTeamID(FGenericTeamId OldTeamID)
{
	ITeamAgentInterface::ConditionalBroadcastTeamChanged(this, OldTeamID, MyTeamID);
}

//~APawn interface
void ACoreCharacter::PossessedBy(AController* NewController)
{
	const FGenericTeamId OldTeamID = MyTeamID;

	Super::PossessedBy(NewController);

	if (UActorComponent* PawnExtComp = FindPawnExtensionComponent(this))
	{
		IPawnExtensionComponentInterface::Execute_HandleControllerChanged(PawnExtComp);
	}

	// Grab the current team ID and listen for future changes
	if (ITeamAgentInterface* ControllerAsTeamProvider = Cast<ITeamAgentInterface>(NewController))
	{
		MyTeamID = ControllerAsTeamProvider->GetGenericTeamId();
		ControllerAsTeamProvider->GetTeamChangedDelegateChecked().AddDynamic(this, &ThisClass::OnControllerChangedTeam);
	}
	ITeamAgentInterface::ConditionalBroadcastTeamChanged(this, OldTeamID, MyTeamID);
}

void ACoreCharacter::UnPossessed()
{
	AController* const OldController = GetController();

	// Stop listening for changes from the old controller
	const FGenericTeamId OldTeamID = MyTeamID;
	if (ITeamAgentInterface* ControllerAsTeamProvider = Cast<ITeamAgentInterface>(OldController))
	{
		ControllerAsTeamProvider->GetTeamChangedDelegateChecked().RemoveAll(this);
	}

	Super::UnPossessed();

	if (UActorComponent* PawnExtComp = FindPawnExtensionComponent(this))
	{
		IPawnExtensionComponentInterface::Execute_HandleControllerChanged(PawnExtComp);
	}

	// Determine what the new team ID should be afterwards
	MyTeamID = DetermineNewTeamAfterPossessionEnds(OldTeamID);
	ITeamAgentInterface::ConditionalBroadcastTeamChanged(this, OldTeamID, MyTeamID);
}

void ACoreCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();

	if (UActorComponent* PawnExtComp = FindPawnExtensionComponent(this))
	{
		IPawnExtensionComponentInterface::Execute_HandleControllerChanged(PawnExtComp);
	}
}

void ACoreCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (UActorComponent* PawnExtComp = FindPawnExtensionComponent(this))
	{
		IPawnExtensionComponentInterface::Execute_HandlePlayerStateReplicated(PawnExtComp);
	}
}
//~End of APawn interface

//~AActor interface
void ACoreCharacter::FellOutOfWorld(const class UDamageType& dmgType)
{
	// Health GameFeature should handle this via interface
	// For now, just call DestroyDueToDeath
	DestroyDueToDeath();
}
//~End of AActor interface

void ACoreCharacter::OnControllerChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam)
{
	const FGenericTeamId OldTeamID = IntegerToGenericTeamId(OldTeam);
	const FGenericTeamId NewTeamID = IntegerToGenericTeamId(NewTeam);
	
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, MyTeamID, this);
	MyTeamID = NewTeamID;
	ITeamAgentInterface::ConditionalBroadcastTeamChanged(this, OldTeamID, NewTeamID);
}

//~ACharacter interface
void ACoreCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		// Update movement mode tags via interface
		SetMovementModeTag(PrevMovementMode, PreviousCustomMode, false);
		SetMovementModeTag(MoveComp->MovementMode, MoveComp->CustomMovementMode, true);
	}
}

void ACoreCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);

	if (UAbilitySystemComponent* ASC = GetCoreAbilitySystemComponent())
	{
		ASC->SetLooseGameplayTagCount(GameplayTags_Core::Status_Crouching, 1);
	}
}

void ACoreCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);

	if (UAbilitySystemComponent* ASC = GetCoreAbilitySystemComponent())
	{
		ASC->SetLooseGameplayTagCount(GameplayTags_Core::Status_Crouching, 0);
	}
}
//~End of ACharacter interface