// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "Characters/CombinedBundleCharacter.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagAssetInterface.h"
#include "Elemental_Interfaces/Public/Interfaces/DeathEventInterface.h"
#include "Elemental_Interfaces/Public/Interfaces/HumanoidReplicationInterface.h"
#include "Elemental_Interfaces/Public/Interfaces/MovementModeTagInterface.h"
#include "Elemental_Interfaces/Public/Interfaces/CharacterGroundInfoProviderInterface.h"
#include "Elemental_Interfaces/Public/Interfaces/TeamAgentInterface.h"
#include "Interfaces/ReplicationGraphCharacterInterface.h"

#include "CoreCharacter.generated.h"

// Forward declarations
// NOTE: 
// - Movement wird über GameFeature_HumanoidMovement hinzugefügt
// - Camera wird über GameFeature_CameraOrchestrator hinzugefügt  
// - GAS wird über separate GameFeatures hinzugefügt (später)
// Diese Komponenten sind NICHT Teil des Characters selbst!
//
// CoreCharacter erweitert CombinedBundleCharacter nur um Lyra-spezifische
// Replication-Strukturen, die von GameFeatures genutzt werden können.

namespace EEndPlayReason { enum Type : int; }
class IRepChangedPropertyTracker;
class FLifetimeProperty;
class UAbilitySystemComponent;
struct FGameplayTag;
struct FGameplayTagContainer;

/**
 * FCoreReplicatedAcceleration: Compressed representation of acceleration
 * Used by Movement GameFeatures for optimized replication
 */
USTRUCT()
struct CORE_CLASSES_API FCoreReplicatedAcceleration
{
	GENERATED_BODY()

	UPROPERTY()
	uint8 AccelXYRadians = 0;	// Direction of XY accel component, quantized to represent [0, 2*pi]

	UPROPERTY()
	uint8 AccelXYMagnitude = 0;	//Accel rate of XY component, quantized to represent [0, MaxAcceleration]

	UPROPERTY()
	int8 AccelZ = 0;	// Raw Z accel rate component, quantized to represent [-MaxAcceleration, MaxAcceleration]
};

/**
 * FCoreSharedRepMovement: The type we use to send FastShared movement updates.
 * Used by Movement GameFeatures for optimized replication when default property replication is skipped.
 */
USTRUCT()
struct CORE_CLASSES_API FCoreSharedRepMovement
{
	GENERATED_BODY()

	FCoreSharedRepMovement();

	bool FillForCharacter(ACharacter* Character);
	bool Equals(const FCoreSharedRepMovement& Other, ACharacter* Character) const;

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

	UPROPERTY(Transient)
	FRepMovement RepMovement;

	UPROPERTY(Transient)
	float RepTimeStamp = 0.0f;

	UPROPERTY(Transient)
	uint8 RepMovementMode = 0;

	UPROPERTY(Transient)
	bool bProxyIsJumpForceApplied = false;

	UPROPERTY(Transient)
	bool bIsCrouched = false;
};

template<>
struct TStructOpsTypeTraits<FCoreSharedRepMovement> : public TStructOpsTypeTraitsBase2<FCoreSharedRepMovement>
{
	enum
	{
		WithNetSerializer = true,
		WithNetSharedSerialization = true,
	};
};

/**
 * ACoreCharacter
 *
 *	The base character pawn class used by this project.
 *	Extends CombinedBundleCharacter with Lyra-style replication structures that can be used by GameFeatures.
 */
UCLASS(MinimalAPI, Config = Game, Meta = (ShortTooltip = "The base character pawn class with Lyra replication features."))
class ACoreCharacter : public ACombinedBundleCharacter,
	public IAbilitySystemInterface,
	public IGameplayTagAssetInterface,
	public ICharacterGroundInfoProviderInterface,
	public IDeathEventInterface,
	public IHumanoidReplicationInterface,
	public IMovementModeTagInterface,
	public ITeamAgentInterface,
	public IReplicationGraphCharacterInterface
{
	GENERATED_BODY()

public:

	CORE_CLASSES_API ACoreCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~AActor interface
	CORE_CLASSES_API virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	CORE_CLASSES_API virtual void PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker) override;
	//~End of AActor interface

	virtual bool UpdateSharedReplication() override;
	virtual void SetReplicatedAcceleration(const FVector& InAcceleration) override;

	UFUNCTION(NetMulticast, unreliable)
	CORE_CLASSES_API void FastSharedReplication(const FCoreSharedRepMovement& SharedRepMovement);

	//~IAbilitySystemInterface
	CORE_CLASSES_API virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~End of IAbilitySystemInterface

	CORE_CLASSES_API void InitAbilityActorInfo(UObject* InOwnerActor, UObject* InAvatarActor);
	CORE_CLASSES_API void ClearActorInfo();

	//~IGameplayTagAssetInterface
	CORE_CLASSES_API virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	CORE_CLASSES_API virtual bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const override;
	CORE_CLASSES_API virtual bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	CORE_CLASSES_API virtual bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	//~End of IGameplayTagAssetInterface

	//~IDeathEventInterface
	CORE_CLASSES_API virtual void OnDeathStarted(AActor* OwningActor) override;
	CORE_CLASSES_API virtual void OnDeathFinished(AActor* OwningActor) override;
	//~End of IDeathEventInterface

	//~IMovementModeTagInterface
	CORE_CLASSES_API virtual void SetMovementModeTag(EMovementMode MovementMode, uint8 CustomMovementMode, bool bTagEnabled) override;
	CORE_CLASSES_API virtual void InitializeMovementModeTags() override;
	CORE_CLASSES_API virtual void SetCrouchingTagCount(int32 Count) override;
	//~End of IMovementModeTagInterface

	//~ITeamAgentInterface
	CORE_CLASSES_API virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	CORE_CLASSES_API virtual FGenericTeamId GetGenericTeamId() const override;
	CORE_CLASSES_API virtual FOnTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	//~End of ITeamAgentInterface

	//~ICharacterGroundInfoProviderInterface
	CORE_CLASSES_API virtual float GetGroundDistance() const override;
	//~End of ICharacterGroundInfoProviderInterface

	//~APawn interface
	CORE_CLASSES_API virtual void PossessedBy(AController* NewController) override;
	CORE_CLASSES_API virtual void UnPossessed() override;
	CORE_CLASSES_API virtual void OnRep_Controller() override;
	CORE_CLASSES_API virtual void OnRep_PlayerState() override;
	//~End of APawn interface

	//~AActor interface
	CORE_CLASSES_API virtual void FellOutOfWorld(const class UDamageType& dmgType) override;
	//~End of AActor interface

	//~ACharacter interface
	CORE_CLASSES_API virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;
	CORE_CLASSES_API virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	CORE_CLASSES_API virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	//~End of ACharacter interface

	UFUNCTION(BlueprintCallable, Category = "Core|Character")
	CORE_CLASSES_API UAbilitySystemComponent* GetCoreAbilitySystemComponent() const;

protected:
	virtual FGenericTeamId DetermineNewTeamAfterPossessionEnds(FGenericTeamId OldTeamID) const
	{
		return FGenericTeamId::NoTeam;
	}

	CORE_CLASSES_API void DisableMovementAndCollision();
	CORE_CLASSES_API void DestroyDueToDeath();

private:
	UFUNCTION()
	CORE_CLASSES_API void OnControllerChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam);

protected:

	UPROPERTY(Transient, ReplicatedUsing = OnRep_ReplicatedAcceleration)
	FCoreReplicatedAcceleration ReplicatedAcceleration;

	UPROPERTY(Transient)
	FCoreSharedRepMovement LastSharedReplication;

	UPROPERTY(ReplicatedUsing = OnRep_MyTeamID)
	FGenericTeamId MyTeamID;

	UPROPERTY()
	FOnTeamIndexChangedDelegate OnTeamChangedDelegate;

private:
	UFUNCTION()
	CORE_CLASSES_API void OnRep_ReplicatedAcceleration();

	UFUNCTION()
	CORE_CLASSES_API void OnRep_MyTeamID(FGenericTeamId OldTeamID);
};
