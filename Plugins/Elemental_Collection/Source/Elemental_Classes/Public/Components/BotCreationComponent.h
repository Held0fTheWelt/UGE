// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "Components/GameStateComponent.h"

#include "BotCreationComponent.generated.h"

class UExperienceDefinition;
class AAIController;

UCLASS(Blueprintable, Abstract)
class ELEMENTAL_CLASSES_API UBotCreationComponent : public UGameStateComponent
{
	GENERATED_BODY()

public:
	UBotCreationComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UActorComponent interface
	virtual void BeginPlay() override;
	//~End of UActorComponent interface

		/** Returns NumBotsToCreate (e.g. for IBotSpawnProvider fallback when OptionsString has no NumBots). */
	int32 GetNumBotsToCreate() const { return NumBotsToCreate; }

private:
	void OnExperienceLoaded(const UExperienceDefinition* Experience);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Gameplay)
	int32 NumBotsToCreate = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Gameplay)
	TSubclassOf<AAIController> BotControllerClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Gameplay)
	TArray<FString> RandomBotNames;

	TArray<FString> RemainingBotNames;

protected:
	UPROPERTY(Transient)
	TArray<TObjectPtr<AAIController>> SpawnedBotList;

	/** Always creates a single bot */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Gameplay)
	virtual void SpawnOneBot();

	/** Deletes the last created bot if possible */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Gameplay)
	virtual void RemoveOneBot();

	/** Spawns bots up to NumBotsToCreate */
	UFUNCTION(BlueprintNativeEvent, BlueprintAuthorityOnly, Category=Gameplay)
	void ServerCreateBots();

#if WITH_SERVER_CODE
public:
	void Cheat_AddBot() { SpawnOneBot(); }
	void Cheat_RemoveBot() { RemoveOneBot(); }

	FString CreateBotName(int32 PlayerIndex);
#endif
};
