// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/CharacterGroundInfoProviderInterface.h"
#include "Interfaces/Control/HumanoidControlInterface.h"
#include "Interfaces/EntityCoreInformationInterface.h"
#include "Interfaces/HumanoidReplicationInterface.h"
#include "Interfaces/MovementModeTagInterface.h"
#include "HumanoidMovementComponent.generated.h"

// Forward declarations
class APawn;
class ACharacter;
class UCharacterMovementComponent;
struct FHitResult;

/**
 * FHumanoidGroundInfo
 *
 *	Information about the ground under the character. Updated only when needed.
 */
USTRUCT(BlueprintType)
struct GAMEFEATURE_HUMANOIDMOVEMENTRUNTIME_API FHumanoidGroundInfo
{
	GENERATED_BODY()

	FHumanoidGroundInfo()
		: LastUpdateFrame(0)
		, GroundDistance(0.0f)
	{}

	uint64 LastUpdateFrame;

	UPROPERTY(BlueprintReadOnly)
	FHitResult GroundHitResult;

	UPROPERTY(BlueprintReadOnly)
	float GroundDistance;
};

UCLASS(ClassGroup = (Custom), BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class GAMEFEATURE_HUMANOIDMOVEMENTRUNTIME_API UHumanoidMovementComponent : public UActorComponent,
	public ICharacterGroundInfoProviderInterface,
	public IHumanoidControlInterface,
	public IEntityCoreInformationInterface
{
	GENERATED_BODY()

public:
	UHumanoidMovementComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	float RunSpeedMultiplier;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Default)
	bool bIsRunning;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Default)
	float WalkSpeed;

	/** Enable optimized replication via IHumanoidReplicationInterface on owner */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Replication")
	bool bUseOptimizedReplication = true;

	/** Frequency of replication updates (in frames). Lower = more frequent */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Replication", meta = (ClampMin = "1"))
	int32 ReplicationUpdateFrequency = 1;

private:
	void Jump_Implementation() override;
	void StopJumping_Implementation() override;
	void Run_Implementation() override;
	void StopRunning_Implementation() override;
	void Move_Implementation(const FVector2D& Value) override;
	void Look_Implementation(const FVector2D& Value) override;

	void UpdateWalkSpeed();
	bool IsCharacterMovementValid() const;

	/** Replication: calls IHumanoidReplicationInterface::UpdateSharedReplication on owner */
	void UpdateReplication();

	/** Returns the pawn/character owner (component must be attached to the character). */
	APawn* GetPawnOwner() const;

	/** Called when the owner pawn is possessed; registers this component with the controller so HumanoidControlInterface is linked. */
	UFUNCTION()
	void OnPossessedByController(AController* InController);

	/**
	 * Returns the current ground info. Calling this updates the ground info if out of date.
	 */
	UFUNCTION(BlueprintCallable, Category = "Humanoid|Movement")
	const FHumanoidGroundInfo& GetGroundInfo();

	//~ICharacterGroundInfoProviderInterface
	virtual float GetGroundDistance() const override;
	//~End of ICharacterGroundInfoProviderInterface

	void SetReplicatedAcceleration(const FVector& InAcceleration);

	/** Handles movement mode changes; delegates to owner IMovementModeTagInterface */
	void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode, EMovementMode NewMovementMode, uint8 NewCustomMode);

	/** Initializes movement mode tags via owner IMovementModeTagInterface */
	void InitializeMovementModeTags();

	/** Delegates to owner IMovementModeTagInterface */
	void SetMovementModeTag(EMovementMode MovementMode, uint8 CustomMovementMode, bool bTagEnabled);

#if WITH_EDITOR
	virtual FName GetFriendlyName_Implementation() const override;
#endif

private:
	UPROPERTY(Transient)
	TObjectPtr<APawn> PawnOwner;

	UPROPERTY(Transient)
	TObjectPtr<UCharacterMovementComponent> CharacterMovement;

	UPROPERTY(Transient)
	int32 ReplicationFrameCounter = 0;

	FHumanoidGroundInfo CachedGroundInfo;

	UPROPERTY(Transient)
	bool bHasReplicatedAcceleration = false;

	//~IEntityCoreInformationInterface
	virtual FOnInformationUpdated& GetOnInformationUpdated() override { return OnInformationUpdated; }
	virtual void SetEntityCoreLoggingEnabled_Implementation(const bool bEnabled) override {}
	virtual FName GetCoreInformationName_Implementation() const override { return FName(TEXT("HumanoidMovement")); }
	virtual TArray<FInformationSet> GetSettingsVariableSets_Implementation() const override;

private:
	FOnInformationUpdated OnInformationUpdated;
};
