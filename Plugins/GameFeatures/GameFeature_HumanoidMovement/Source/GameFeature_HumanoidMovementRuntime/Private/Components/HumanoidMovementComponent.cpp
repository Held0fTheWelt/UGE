// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/HumanoidMovementComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Components/CapsuleComponent.h"

#include "Interfaces/HumanoidReplicationInterface.h"
#include "Interfaces/MovementModeTagInterface.h"
#include "Interfaces/PossessedByControllerInterface.h"
#include "Interfaces/RegisterComponentInterface.h"

#include "Libs/CoreInformationFunctionLibrary.h"
#include "Structs/InformationSet.h"

#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"
#include "Logging/Logging_GF_HumanoidMovement.h"

namespace HumanoidMovement
{
	static float GroundTraceDistance = 100000.0f;
	FAutoConsoleVariableRef CVar_GroundTraceDistance(TEXT("HumanoidMovement.GroundTraceDistance"), GroundTraceDistance, TEXT("Distance to trace down when generating ground information."), ECVF_Cheat);
}

UHumanoidMovementComponent::UHumanoidMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.TickGroup = TG_PrePhysics;

	RunSpeedMultiplier = 1.5f;
	bUseOptimizedReplication = true;
	ReplicationUpdateFrequency = 1;
}

void UHumanoidMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	PawnOwner = Cast<APawn>(GetOwner());
	if (!PawnOwner)
	{
		UE_LOGFMT(Log_HumanoidMovement, Warning, "{Time}: {Line} {Class}: Owner is not a Pawn",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	CharacterMovement = PawnOwner->FindComponentByClass<UCharacterMovementComponent>();
	if (!IsCharacterMovementValid())
	{
		UE_LOGFMT(Log_HumanoidMovement, Warning, "{Time}: {Line} {Class}: CharacterMovementComponent not found",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	WalkSpeed = CharacterMovement->MaxWalkSpeed;
	UE_LOGFMT(Log_HumanoidMovement, VeryVerbose, "{Time}: {Line} {Class}: WalkSpeed initialized to {WalkSpeed}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("WalkSpeed", WalkSpeed));

	if (bUseOptimizedReplication)
	{
		ReplicationFrameCounter = 0;
	}

	// Register with controller on possess so HumanoidControlInterface is linked and Move/Jump/Run receive input
	if (PawnOwner->GetClass()->ImplementsInterface(UPossessedByControllerInterface::StaticClass()))
	{
		IPossessedByControllerInterface* PossessInterface = Cast<IPossessedByControllerInterface>(PawnOwner);
		if (PossessInterface)
		{
			PossessInterface->GetOnPawnPossessedByController().AddDynamic(this, &UHumanoidMovementComponent::OnPossessedByController);
		}
	}
	// If already possessed, register immediately
	OnPossessedByController(PawnOwner->GetController());
}

void UHumanoidMovementComponent::OnPossessedByController(AController* InController)
{
	AController* Controller = InController ? InController : (PawnOwner ? PawnOwner->GetController() : nullptr);
	if (!Controller)
	{
		return;
	}
	if (Controller->GetClass()->ImplementsInterface(URegisterComponentInterface::StaticClass()))
	{
		IRegisterComponentInterface::Execute_RegisterComponentWithInterface(Controller, this);
		UE_LOGFMT(Log_HumanoidMovement, Verbose, "{Time}: {Line} {Class}: Registered HumanoidControlInterface with controller {Controller}",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME), ("Controller", *Controller->GetName()));
	}
}

void UHumanoidMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bUseOptimizedReplication && PawnOwner && GetOwner()->GetLocalRole() == ROLE_Authority)
	{
		ReplicationFrameCounter++;
		if (ReplicationFrameCounter >= ReplicationUpdateFrequency)
		{
			ReplicationFrameCounter = 0;
			UpdateReplication();
		}
	}
}

void UHumanoidMovementComponent::Jump_Implementation()
{
	if (!PawnOwner) return;
	ACharacter* Character = Cast<ACharacter>(PawnOwner);
	if (!Character)
	{
		UE_LOGFMT(Log_HumanoidMovement, Warning, "{Time}: {Line} {Class}: Owner is not a Character", ("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}
	Character->Jump();
}

void UHumanoidMovementComponent::StopJumping_Implementation()
{
	if (!PawnOwner) return;
	ACharacter* Character = Cast<ACharacter>(PawnOwner);
	if (!Character) return;
	Character->StopJumping();
}

void UHumanoidMovementComponent::Run_Implementation()
{
	bIsRunning = true;
	UpdateWalkSpeed();
}

void UHumanoidMovementComponent::StopRunning_Implementation()
{
	bIsRunning = false;
	UpdateWalkSpeed();
}

void UHumanoidMovementComponent::Move_Implementation(const FVector2D& Value)
{
	if (!PawnOwner) return;
	ACharacter* Character = Cast<ACharacter>(PawnOwner);
	if (!Character) return;
	Character->AddMovementInput(Character->GetActorForwardVector(), Value.Y);
	Character->AddMovementInput(Character->GetActorRightVector(), Value.X);
}

void UHumanoidMovementComponent::Look_Implementation(const FVector2D& Value)
{
	// Optional: animate humanoid head / camera
	(void)Value;
}

void UHumanoidMovementComponent::UpdateWalkSpeed()
{
	if (!IsCharacterMovementValid()) return;
	CharacterMovement->MaxWalkSpeed = bIsRunning ? (WalkSpeed * RunSpeedMultiplier) : WalkSpeed;
}

bool UHumanoidMovementComponent::IsCharacterMovementValid() const
{
	return IsValid(CharacterMovement);
}

void UHumanoidMovementComponent::UpdateReplication()
{
	if (!PawnOwner || GetOwner()->GetLocalRole() != ROLE_Authority) return;
	if (IHumanoidReplicationInterface* RepInterface = Cast<IHumanoidReplicationInterface>(GetOwner()))
	{
		RepInterface->UpdateSharedReplication();
	}
}

APawn* UHumanoidMovementComponent::GetPawnOwner() const
{
	return PawnOwner;
}

float UHumanoidMovementComponent::GetGroundDistance() const
{
	const FHumanoidGroundInfo& Info = const_cast<UHumanoidMovementComponent*>(this)->GetGroundInfo();
	return Info.GroundDistance;
}

const FHumanoidGroundInfo& UHumanoidMovementComponent::GetGroundInfo()
{
	if (!PawnOwner || !CharacterMovement || (GFrameCounter == CachedGroundInfo.LastUpdateFrame))
	{
		return CachedGroundInfo;
	}

	if (CharacterMovement->MovementMode == MOVE_Walking)
	{
		CachedGroundInfo.GroundHitResult = CharacterMovement->CurrentFloor.HitResult;
		CachedGroundInfo.GroundDistance = 0.0f;
	}
	else
	{
		const UCapsuleComponent* CapsuleComp = CharacterMovement->GetCharacterOwner()->GetCapsuleComponent();
		if (CapsuleComp)
		{
			const float CapsuleHalfHeight = CapsuleComp->GetUnscaledCapsuleHalfHeight();
			const ECollisionChannel CollisionChannel = (CharacterMovement->UpdatedComponent ? CharacterMovement->UpdatedComponent->GetCollisionObjectType() : ECC_Pawn);
			const FVector TraceStart(CharacterMovement->GetActorLocation());
			const FVector TraceEnd(TraceStart.X, TraceStart.Y, (TraceStart.Z - HumanoidMovement::GroundTraceDistance - CapsuleHalfHeight));

			FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(HumanoidMovementComponent_GetGroundInfo), false, CharacterMovement->GetCharacterOwner());
			FCollisionResponseParams ResponseParam;
			CharacterMovement->InitCollisionParams(QueryParams, ResponseParam);

			FHitResult HitResult;
			GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, CollisionChannel, QueryParams, ResponseParam);

			CachedGroundInfo.GroundHitResult = HitResult;
			CachedGroundInfo.GroundDistance = HumanoidMovement::GroundTraceDistance;

			if (CharacterMovement->MovementMode == MOVE_NavWalking)
			{
				CachedGroundInfo.GroundDistance = 0.0f;
			}
			else if (HitResult.bBlockingHit)
			{
				CachedGroundInfo.GroundDistance = FMath::Max((HitResult.Distance - CapsuleHalfHeight), 0.0f);
			}
		}
	}

	CachedGroundInfo.LastUpdateFrame = GFrameCounter;
	return CachedGroundInfo;
}

void UHumanoidMovementComponent::SetReplicatedAcceleration(const FVector& InAcceleration)
{
	bHasReplicatedAcceleration = true;
	if (IHumanoidReplicationInterface* RepInterface = Cast<IHumanoidReplicationInterface>(GetOwner()))
	{
		RepInterface->SetReplicatedAcceleration(InAcceleration);
	}
}

void UHumanoidMovementComponent::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode, EMovementMode NewMovementMode, uint8 NewCustomMode)
{
	if (IMovementModeTagInterface* MovementTagInterface = Cast<IMovementModeTagInterface>(GetOwner()))
	{
		MovementTagInterface->SetMovementModeTag(PrevMovementMode, PreviousCustomMode, false);
		MovementTagInterface->SetMovementModeTag(NewMovementMode, NewCustomMode, true);
	}
}

void UHumanoidMovementComponent::InitializeMovementModeTags()
{
	if (IMovementModeTagInterface* MovementTagInterface = Cast<IMovementModeTagInterface>(GetOwner()))
	{
		MovementTagInterface->InitializeMovementModeTags();
	}
}

void UHumanoidMovementComponent::SetMovementModeTag(EMovementMode MovementMode, uint8 CustomMovementMode, bool bTagEnabled)
{
	if (IMovementModeTagInterface* MovementTagInterface = Cast<IMovementModeTagInterface>(GetOwner()))
	{
		MovementTagInterface->SetMovementModeTag(MovementMode, CustomMovementMode, bTagEnabled);
	}
}

#if WITH_EDITOR
FName UHumanoidMovementComponent::GetFriendlyName_Implementation() const
{
	return GetFName();
}
#endif

TArray<FInformationSet> UHumanoidMovementComponent::GetSettingsVariableSets_Implementation() const
{
	TArray<FInformationSet> InformationSets;
	FInformationSet HumanoidMovementInfoSet = UCoreInformationFunctionLibrary::GetInformationSet("Humanoid Movement", "", "");
	UCoreInformationFunctionLibrary::AddInformationValue(HumanoidMovementInfoSet, "Walk Speed", FString::SanitizeFloat(WalkSpeed), EValueType::VT_STRING);
	UCoreInformationFunctionLibrary::AddInformationValue(HumanoidMovementInfoSet, "Run SpeedModifier", FString::SanitizeFloat(RunSpeedMultiplier), EValueType::VT_STRING);
	UCoreInformationFunctionLibrary::AddInformationValue(HumanoidMovementInfoSet, "Is Running", bIsRunning ? "True" : "False", EValueType::VT_BOOL);
	InformationSets.Add(HumanoidMovementInfoSet);
	return InformationSets;
}
