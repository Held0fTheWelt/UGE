// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CombinedBundleActorComponent.h"
#include "EngineControlUnitComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(Log_EngineControlUnit, Log, All);

/**
 *
 */
UCLASS()
class CORE_CLASSES_API UEngineControlUnitComponent : public UCombinedBundleActorComponent
{
	GENERATED_BODY()


public:
	UEngineControlUnitComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	virtual void OnEngineActiveStateChanged(bool IsOn);
	UFUNCTION()
	virtual void OnSecondEngineActiveStateChanged(bool IsOn);

	virtual void SetupLoadedAsset(UObject* InOwner, UObject* LoadedObject) override;

	virtual TArray<FInformationSet> GetSettingsVariableSets_Implementation() const override;

protected:	
	UPROPERTY(Transient)
	bool bIsEngineControlActive;
	UPROPERTY(Transient)
	bool bIsEngineActive;
	UPROPERTY(Transient)
	bool bIsSecondEngineActive;

private:
	void BindEngineControl();
};
