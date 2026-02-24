// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "PlayerSpawningGameStateComponent.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENTAL_PLAYER_API UPlayerSpawningGameStateComponent : public UGameStateComponent
{
	GENERATED_BODY()

public:
	UPlayerSpawningGameStateComponent(const FObjectInitializer& ObjectInitializer);

	/** UActorComponent */
	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	/** ~UActorComponent */

protected:
	// Utility
	APlayerStart* GetFirstRandomUnoccupiedPlayerStart(class AController* Controller, const TArray<class APlayerStart_Base*>& FoundStartPoints) const;

	virtual AActor* OnChoosePlayerStart(class AController* Player, TArray<class APlayerStart_Base*>& PlayerStarts) { return nullptr; }
	virtual void OnFinishRestartPlayer(class AController* Player, const FRotator& StartRotation) {}

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = OnFinishRestartPlayer))
	void K2_OnFinishRestartPlayer(class AController* Player, const FRotator& StartRotation);

private:

	/** We proxy these calls from ALyraGameMode, to this component so that each experience can more easily customize the respawn system they want. */
	AActor* ChoosePlayerStart(class AController* Player);
	bool ControllerCanRestart(class AController* Player);
	void FinishRestartPlayer(class AController* NewPlayer, const FRotator& StartRotation);
	friend class ALyraGameMode;
	/** ~ALyraGameMode */

	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<class APlayerStart_Base>> CachedPlayerStarts;

private:
	void OnLevelAdded(class ULevel* InLevel, class UWorld* InWorld);
	void HandleOnActorSpawned(class AActor* SpawnedActor);

#if WITH_EDITOR
	APlayerStart* FindPlayFromHereStart(class AController* Player);
#endif	
};
