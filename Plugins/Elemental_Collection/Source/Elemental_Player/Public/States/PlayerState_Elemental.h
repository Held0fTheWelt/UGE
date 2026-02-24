// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "AbilitySystemInterface.h"
#include "States/ModularPlayerState.h"
#include "Interfaces/TeamAgentInterface.h"
#include "System/GameplayTagStack.h"
#include "Structs/VerbMessageTypes.h"
#include "Enums/PlayerConnectionType.h"
#include "PlayerState_Elemental.generated.h"

class AController;
class APlayerState;
class FName;
class UAbilitySystemComponent;
class UObject;
class UExperienceDefinition;
struct FFrame;
struct FGameplayTag;



/**
 * APlayerState_Elemental
 *
 *	Base player state class used by WarCollection.
 *	Adapted from ALyraPlayerState.
 */
UCLASS(MinimalAPI, Config = Game)
class APlayerState_Elemental : public AModularPlayerState, public IAbilitySystemInterface, public ITeamAgentInterface
{
	GENERATED_BODY()

public:
	ELEMENTAL_PLAYER_API APlayerState_Elemental(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "Elemental|PlayerState")
	ELEMENTAL_PLAYER_API class APlayerController* GetUnifiedPlayerController() const;

	UFUNCTION(BlueprintCallable, Category = "Elemental|PlayerState")
	class UActorComponent* GetPawnAbilitySystemComponent() const { return AbilitySystemComponent; }
	ELEMENTAL_PLAYER_API virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	ELEMENTAL_PLAYER_API void InitAbilityActorInfo(UObject* InOwnerActor, UObject* InAvatarActor);
	ELEMENTAL_PLAYER_API void ClearActorInfo();

	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }

	ELEMENTAL_PLAYER_API void SetPawnData(const UObject* InPawnData);

	// Initialize the ability system component
	ELEMENTAL_PLAYER_API void InitializeAbilitySystem();

	// Reset the ability system component
	ELEMENTAL_PLAYER_API void ResetAbilitySystem();

	//~AActor interface
	ELEMENTAL_PLAYER_API virtual void PreInitializeComponents() override;
	ELEMENTAL_PLAYER_API virtual void PostInitializeComponents() override;
	//~End of AActor interface

	//~APlayerState interface
	ELEMENTAL_PLAYER_API virtual void Reset() override;
	ELEMENTAL_PLAYER_API virtual void ClientInitialize(AController* C) override;
	ELEMENTAL_PLAYER_API virtual void CopyProperties(APlayerState* PlayerState) override;
	ELEMENTAL_PLAYER_API virtual void OnDeactivated() override;
	ELEMENTAL_PLAYER_API virtual void OnReactivated() override;
	//~End of APlayerState interface

	//~ITeamAgentInterface interface
	ELEMENTAL_PLAYER_API virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	ELEMENTAL_PLAYER_API virtual FGenericTeamId GetGenericTeamId() const override;
	ELEMENTAL_PLAYER_API virtual FOnTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	//~End of ITeamAgentInterface interface

	ELEMENTAL_PLAYER_API void SetPlayerConnectionType(EPlayerConnectionType NewType);
	EPlayerConnectionType GetPlayerConnectionType() const { return MyPlayerConnectionType; }

	/** Returns the Squad ID of the squad the player belongs to. */
	UFUNCTION(BlueprintCallable)
	int32 GetSquadId() const
	{
		return MySquadID;
	}

	/** Returns the Team ID of the team the player belongs to. */
	UFUNCTION(BlueprintCallable)
	int32 GetTeamId() const
	{
		return GenericTeamIdToInteger(MyTeamID);
	}

	ELEMENTAL_PLAYER_API void SetSquadID(int32 NewSquadID);

	// Adds a specified number of stacks to the tag (does nothing if StackCount is below 1)
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Teams)
	ELEMENTAL_PLAYER_API void AddStatTagStack(FGameplayTag Tag, int32 StackCount);

	// Removes a specified number of stacks from the tag (does nothing if StackCount is below 1)
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Teams)
	ELEMENTAL_PLAYER_API void RemoveStatTagStack(FGameplayTag Tag, int32 StackCount);

	// Returns the stack count of the specified tag (or 0 if the tag is not present)
	UFUNCTION(BlueprintCallable, Category=Teams)
	ELEMENTAL_PLAYER_API int32 GetStatTagStackCount(FGameplayTag Tag) const;

	// Returns true if there is at least one stack of the specified tag
	UFUNCTION(BlueprintCallable, Category=Teams)
	ELEMENTAL_PLAYER_API bool HasStatTag(FGameplayTag Tag) const;

	// Send a message to just this player
	// (use only for client notifications like accolades, quest toasts, etc... that can handle being occasionally lost)
	UFUNCTION(Client, Unreliable, BlueprintCallable, Category = "Elemental|PlayerState")
	ELEMENTAL_PLAYER_API void ClientBroadcastMessage(const FVerbMessage Message);

	// Gets the replicated view rotation of this player, used for spectating
	ELEMENTAL_PLAYER_API FRotator GetReplicatedViewRotation() const;

	// Sets the replicated view rotation, only valid on the server
	ELEMENTAL_PLAYER_API void SetReplicatedViewRotation(const FRotator& NewRotation);

private:
	ELEMENTAL_PLAYER_API void OnExperienceLoaded(const UExperienceDefinition* CurrentExperience);

protected:
	UFUNCTION()
	ELEMENTAL_PLAYER_API void OnRep_PawnData();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	UPROPERTY(ReplicatedUsing = OnRep_PawnData)
	TObjectPtr<const UObject> PawnData;

private:

	// The ability system component sub-object used by player characters.
	UPROPERTY(VisibleAnywhere, Category = "Elemental|PlayerState")
	TObjectPtr<UActorComponent> AbilitySystemComponent;

	// Health attribute set used by this actor.
	UPROPERTY()
	TObjectPtr<const class UObject> HealthSet;
	// Combat attribute set used by this actor.
	UPROPERTY()
	TObjectPtr<const class UObject> CombatSet;

	UPROPERTY(Replicated)
	EPlayerConnectionType MyPlayerConnectionType;

	UPROPERTY()
	FOnTeamIndexChangedDelegate OnTeamChangedDelegate;

	UPROPERTY(ReplicatedUsing=OnRep_MyTeamID)
	FGenericTeamId MyTeamID;

	UPROPERTY(ReplicatedUsing=OnRep_MySquadID)
	int32 MySquadID;

	UPROPERTY(Replicated)
	FGameplayTagStackContainer StatTags;

	UPROPERTY(Replicated)
	FRotator ReplicatedViewRotation;

private:
	UFUNCTION()
	ELEMENTAL_PLAYER_API void OnRep_MyTeamID(FGenericTeamId OldTeamID);

	UFUNCTION()
	ELEMENTAL_PLAYER_API void OnRep_MySquadID();
};
