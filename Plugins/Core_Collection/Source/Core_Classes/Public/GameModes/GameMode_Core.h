// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Interfaces/GameMode_CoreInterface.h"
#include "Interfaces/GameModeInterface.h"
#include "Interfaces/ExperienceManagerInterface.h"
#include "GameMode_Core.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(Log_GameMode_Core, Log, All);

class UObject;
struct FPrimaryAssetId;

/**
 * Post login event, triggered when a player or bot joins the game as well as after seamless and non seamless travel
 *
 * This is called after the player has finished initialization
 */
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCoreGameModePlayerInitialized, AGameModeBase* /*GameMode*/, AController* /*NewPlayer*/);

/**
 * AGameMode_Core
 *
 *	The base game mode class used by this project.
 *	Adapted from ALyraGameMode.
 */
UCLASS()
/** Implements IGameModeInterface (Elemental) so Elemental code (e.g. PlayerState_Elemental) can get PawnData via interface. */
class CORE_CLASSES_API AGameMode_Core : public AGameModeBase, public IGameMode_CoreInterface, public IGameModeInterface
{
	GENERATED_BODY()

public:
AGameMode_Core(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Returns pawn data for the controller (from PawnDataProviderSubsystem). Use GetPawnDataForController(AController*) when calling via IGameModeInterface. */
	const UObject* GetPawnDataForControllerImpl(const AController* InController) const;

	/** C++ helper when you have const AController*; not exposed to Blueprint (use interface GetPawnDataForController(AController*) for UObject*). */
	const UObject* GetPawnDataForController(const AController* InController) const { return GetPawnDataForControllerImpl(InController); }

	//~IGameModeInterface (Elemental)
	UFUNCTION(BlueprintCallable, Category = "Core|Pawn")
	const UObject* GetPawnDataForController(AController* Controller) const override;
	void GenericPlayerInitialization(AController* NewController) override;

	//~AGameModeBase interface
virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;
virtual bool ShouldSpawnAtStartSpot(AController* Player) override;
virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
virtual void FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation) override;
virtual bool PlayerCanRestart_Implementation(APlayerController* Player) override;
virtual void InitGameState() override;
virtual void BeginPlay() override;
virtual bool UpdatePlayerStartSpot(AController* Player, const FString& Portal, FString& OutErrorMessage) override;
virtual void FailedToRestartPlayer(AController* NewPlayer) override;
	//~End of AGameModeBase interface

	// Restart (respawn) the specified player or bot next frame
	// - If bForceReset is true, the controller will be reset this frame (abandoning the currently possessed pawn, if any)
	UFUNCTION(BlueprintCallable)
void RequestPlayerRestartNextFrame(AController* Controller, bool bForceReset = false);

	// Agnostic version of PlayerCanRestart that can be used for both player bots and players
virtual bool ControllerCanRestart(AController* Controller);

	// Delegate called on player initialization, described above 
	FOnCoreGameModePlayerInitialized OnGameModePlayerInitialized;

protected:	
	void OnExperienceLoaded(const class UExperienceDefinition* CurrentExperience);
	bool IsExperienceLoaded() const;

	void OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId, const FString& ExperienceIdSource);

	void HandleMatchAssignmentIfNotExpectingOne();

private:
	virtual void SpawnNewActorAtLocation_Implementation(class AController* Instigator, const FTransform& InTransform) override;
};
