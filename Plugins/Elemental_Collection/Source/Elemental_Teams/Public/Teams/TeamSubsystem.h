// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "Subsystems/WorldSubsystem.h"

#include "TeamSubsystem.generated.h"

class AActor;
class APlayerState;
class ATeamInfoBase;
class ATeamPrivateInfo;
class ATeamPublicInfo;
class FSubsystemCollectionBase;
class UTeamDisplayAsset;
struct FFrame;
struct FGameplayTag;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTeamDisplayAssetChangedDelegate, const UTeamDisplayAsset*, DisplayAsset);

USTRUCT()
struct FTeamTrackingInfo
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<ATeamPublicInfo> PublicInfo = nullptr;

	UPROPERTY()
	TObjectPtr<ATeamPrivateInfo> PrivateInfo = nullptr;

	UPROPERTY()
	TObjectPtr<UTeamDisplayAsset> DisplayAsset = nullptr;

	UPROPERTY()
	FOnTeamDisplayAssetChangedDelegate OnTeamDisplayAssetChanged;

public:
	void SetTeamInfo(ATeamInfoBase* Info);
	void RemoveTeamInfo(ATeamInfoBase* Info);
};

// Result of comparing the team affiliation for two actors
UENUM(BlueprintType)
enum class ETeamComparison : uint8
{
	// Both actors are members of the same team
	OnSameTeam,

	// The actors are members of opposing teams
	DifferentTeams,

	// One (or both) of the actors was invalid or not part of any team
	InvalidArgument
};

/** A subsystem for easy access to team information for team-based actors (e.g., pawns or player states) */
UCLASS(MinimalAPI)
class UTeamSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	ELEMENTAL_TEAMS_API UTeamSubsystem();

	//~USubsystem interface
	ELEMENTAL_TEAMS_API virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	ELEMENTAL_TEAMS_API virtual void Deinitialize() override;
	//~End of USubsystem interface

	// Tries to registers a new team
	ELEMENTAL_TEAMS_API bool RegisterTeamInfo(ATeamInfoBase* TeamInfo);

	// Tries to unregister a team, will return false if it didn't work
	ELEMENTAL_TEAMS_API bool UnregisterTeamInfo(ATeamInfoBase* TeamInfo);

	// Changes the team associated with this actor if possible
	// Note: This function can only be called on the authority
	ELEMENTAL_TEAMS_API bool ChangeTeamForActor(AActor* ActorToChange, int32 NewTeamId);

	// Returns the team this object belongs to, or INDEX_NONE if it is not part of a team
	ELEMENTAL_TEAMS_API int32 FindTeamFromObject(const UObject* TestObject) const;

	// Returns the associated player state for this actor, or nullptr if it is not associated with a player
	ELEMENTAL_TEAMS_API const APlayerState* FindPlayerStateFromActor(const AActor* PossibleTeamActor) const;

	// Returns the team this object belongs to, or INDEX_NONE if it is not part of a team
	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category=Teams, meta=(Keywords="Get"))
	ELEMENTAL_TEAMS_API void FindTeamFromActor(const UObject* TestActor, bool& bIsPartOfTeam, int32& TeamId) const;

	// Compare the teams of two actors and returns a value indicating if they are on same teams, different teams, or one/both are invalid
	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category=Teams, meta=(ExpandEnumAsExecs=ReturnValue))
	ELEMENTAL_TEAMS_API ETeamComparison CompareTeams(const UObject* A, const UObject* B, int32& TeamIdA, int32& TeamIdB) const;

	// Compare the teams of two actors and returns a value indicating if they are on same teams, different teams, or one/both are invalid
	ELEMENTAL_TEAMS_API ETeamComparison CompareTeams(const UObject* A, const UObject* B) const;

	// Returns true if the instigator can damage the target, taking into account the friendly fire settings
	ELEMENTAL_TEAMS_API bool CanCauseDamage(const UObject* Instigator, const UObject* Target, bool bAllowDamageToSelf = true) const;

	// Adds a specified number of stacks to the tag (does nothing if StackCount is below 1)
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Teams)
	ELEMENTAL_TEAMS_API void AddTeamTagStack(int32 TeamId, FGameplayTag Tag, int32 StackCount);

	// Removes a specified number of stacks from the tag (does nothing if StackCount is below 1)
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Teams)
	ELEMENTAL_TEAMS_API void RemoveTeamTagStack(int32 TeamId, FGameplayTag Tag, int32 StackCount);

	// Returns the stack count of the specified tag (or 0 if the tag is not present)
	UFUNCTION(BlueprintCallable, Category=Teams)
	ELEMENTAL_TEAMS_API int32 GetTeamTagStackCount(int32 TeamId, FGameplayTag Tag) const;

	// Returns true if there is at least one stack of the specified tag
	UFUNCTION(BlueprintCallable, Category=Teams)
	ELEMENTAL_TEAMS_API bool TeamHasTag(int32 TeamId, FGameplayTag Tag) const;

	// Returns true if the specified team exists
	UFUNCTION(BlueprintCallable, Category=Teams)
	ELEMENTAL_TEAMS_API bool DoesTeamExist(int32 TeamId) const;

	// Gets the team display asset for the specified team, from the perspective of the specified team
	// (You have to specify a viewer too, in case the game mode is in a 'local player is always blue team' sort of situation)
	UFUNCTION(BlueprintCallable, Category=Teams)
	ELEMENTAL_TEAMS_API UTeamDisplayAsset* GetTeamDisplayAsset(int32 TeamId, int32 ViewerTeamId);

	// Gets the team display asset for the specified team, from the perspective of the specified team
	// (You have to specify a viewer too, in case the game mode is in a 'local player is always blue team' sort of situation)
	UFUNCTION(BlueprintCallable, Category = Teams)
	ELEMENTAL_TEAMS_API UTeamDisplayAsset* GetEffectiveTeamDisplayAsset(int32 TeamId, UObject* ViewerTeamAgent);

	// Gets the list of teams
	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category=Teams)
	ELEMENTAL_TEAMS_API TArray<int32> GetTeamIDs() const;

	// Called when a team display asset has been edited, causes all team color observers to update
	ELEMENTAL_TEAMS_API void NotifyTeamDisplayAssetModified(UTeamDisplayAsset* ModifiedAsset);

	// Register for a team display asset notification for the specified team ID
	ELEMENTAL_TEAMS_API FOnTeamDisplayAssetChangedDelegate& GetTeamDisplayAssetChangedDelegate(int32 TeamId);

private:
	UPROPERTY()
	TMap<int32, FTeamTrackingInfo> TeamMap;
};
