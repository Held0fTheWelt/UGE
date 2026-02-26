// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Components/GameStateComponent.h"
#include "Interfaces/ExperienceManagerInterface.h"
#include "LoadingProcessInterface.h"

#include "ExperienceManagerComponent.generated.h"

namespace UE::GameFeatures { struct FResult; }

class UExperienceDefinition;

DECLARE_LOG_CATEGORY_EXTERN(Log_Experience, Log, All);

enum class EExperienceLoadState
{
	Unloaded,
	Loading,
	LoadingGameFeatures,
	LoadingChaosTestingDelay,
	ExecutingActions,
	Loaded,
	Deactivating
};

/**
 * Manages loading and activation of experiences. Implements IExperienceManagerInterface (Elemental) for framework communication.
 */
UCLASS(MinimalApi)
class UExperienceManagerComponent final : public UGameStateComponent, public ILoadingProcessInterface, public IExperienceManagerInterface
{
	GENERATED_BODY()

public:

	GAMEFEATURE_EXPERIENCESYSTEMRUNTIME_API UExperienceManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//~ILoadingProcessInterface
	GAMEFEATURE_EXPERIENCESYSTEMRUNTIME_API virtual bool ShouldShowLoadingScreen(FString& OutReason) const override;

	//~IExperienceManagerInterface
	GAMEFEATURE_EXPERIENCESYSTEMRUNTIME_API virtual UExperienceManagerComponent* GetExperienceManagerComponent() const override;
	GAMEFEATURE_EXPERIENCESYSTEMRUNTIME_API virtual void CallOrRegister_OnExperienceLoaded(FOnExperienceLoaded::FDelegate&& Delegate) override;
	GAMEFEATURE_EXPERIENCESYSTEMRUNTIME_API virtual bool IsExperienceLoaded() const override;
	GAMEFEATURE_EXPERIENCESYSTEMRUNTIME_API virtual const UExperienceDefinition* GetCurrentExperience() const override;
	GAMEFEATURE_EXPERIENCESYSTEMRUNTIME_API virtual void CallOrRegister_OnExperienceLoaded_LowPriority(FOnExperienceLoaded::FDelegate&& Delegate) override;
	GAMEFEATURE_EXPERIENCESYSTEMRUNTIME_API virtual FSoftObjectPath GetDefaultPawnDataPathFromCurrentExperience() const override;

	GAMEFEATURE_EXPERIENCESYSTEMRUNTIME_API void SetCurrentExperience(FPrimaryAssetId ExperienceId);

	GAMEFEATURE_EXPERIENCESYSTEMRUNTIME_API void CallOrRegister_OnExperienceLoaded_HighPriority(FOnExperienceLoaded::FDelegate&& Delegate);

	GAMEFEATURE_EXPERIENCESYSTEMRUNTIME_API const UExperienceDefinition* GetCurrentExperienceChecked() const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UFUNCTION()
	void OnRep_CurrentExperience();

	void StartExperienceLoad();
	void OnExperienceLoadComplete();
	void OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result);
	void OnExperienceFullLoadCompleted();

	void OnActionDeactivationCompleted();
	void OnAllActionsDeactivated();

private:
	UPROPERTY(ReplicatedUsing = OnRep_CurrentExperience)
	TObjectPtr<const UExperienceDefinition> CurrentExperience;

	EExperienceLoadState LoadState = EExperienceLoadState::Unloaded;

	int32 NumGameFeaturePluginsLoading = 0;
	TArray<FString> GameFeaturePluginURLs;

	int32 NumObservedPausers = 0;
	int32 NumExpectedPausers = 0;

	FOnExperienceLoaded OnExperienceLoaded_HighPriority;
	FOnExperienceLoaded OnExperienceLoaded;
	FOnExperienceLoaded OnExperienceLoaded_LowPriority;
};
