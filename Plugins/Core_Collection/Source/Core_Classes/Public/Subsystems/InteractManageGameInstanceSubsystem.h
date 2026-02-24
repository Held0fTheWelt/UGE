// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "InteractManageGameInstanceSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class CORE_CLASSES_API UInteractManageGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	void InteractedWithActor(class AActor* InteractedActor, class AActor* Interactor, class AController* InteractorInstigator);
	void AbandonOldPawn(class AActor* InteractedActor, class AController* InteractorInstigator);
	void SpawnNewDefaultPawn(class AController* InteractorInstigator);
	void OccupyGivenPawn(class AActor* InteractedActor, class AController* InteractorInstigator);
	void LookupDefinedCollectionDummies(class AActor* InteractedActor, class AController* InteractorInstigator);
	
	void StoppedInteractWithActor(class AActor* InteractedActor, class AActor* Interactor, class AController* InteractorInstigator);
	
	bool CheckInputValues(class AActor* InteractedActor, class AActor* Interactor, class AController* InteractorInstigator) const;
	void SpawnNewDefinedPawn(class AActor* InteractedDummyActor, TSubclassOf<class AActor> DefinedActor, class AController* InteractorInstigator);


	void AddPrimaryDataAsset(TSoftObjectPtr<class UActorCollectionDataAsset> PrimaryDataAsset);

protected:
	/** Implement this for initialization of instances of the system */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/** Implement this for deinitialization of instances of the system */
	virtual void Deinitialize() override;


private:
	UPROPERTY(Transient)
	TArray<TSoftObjectPtr<class UActorCollectionDataAsset>> ActorCollectionDataAssets;
};
