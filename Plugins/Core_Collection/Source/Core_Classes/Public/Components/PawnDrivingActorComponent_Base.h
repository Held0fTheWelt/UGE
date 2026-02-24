// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CombinedBundleActorComponent.h"
#include "PawnDrivingActorComponent_Base.generated.h"

/**
 * 
 */
UCLASS()
class CORE_CLASSES_API UPawnDrivingActorComponent_Base : public UCombinedBundleActorComponent
{
	GENERATED_BODY()
	
public:
	UPawnDrivingActorComponent_Base(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(Transient)
	TObjectPtr<class APawn> PawnOwner;
	UPROPERTY(Transient)
	TObjectPtr<class AController> PawnController;

	UPROPERTY(BlueprintReadOnly, Category = "Pawn Driving")
	bool bDoNotRegisterOnPossessControl;
protected:
	virtual void BeginPlay() override;
	bool IsPawnOwnerValid() const;
	bool IsPawnControllerValid() const;

	virtual void ProcessPossess();

	virtual TArray<FInformationSet> GetSettingsVariableSets_Implementation() const override;
	
private:
	UFUNCTION()
	void OnPossessFired(class AController* InController);
	void OnNextTick();

};
