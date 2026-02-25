// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnDrivingActorComponent_Base.h"
#include "Interfaces/InteractInterface.h"
#include "Interfaces/Control/InteractControlInterface.h"
#include "Interfaces/ShapeUpdatedInterface.h"
#include "InteractCollisionComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GAMEFEATURE_INTERACTRUNTIME_API UInteractCollisionComponent : 
	public UPawnDrivingActorComponent_Base, 
	public IInteractControlInterface,
	public IShapeUpdatedInterface
{
	GENERATED_BODY()

public:	
	UInteractCollisionComponent(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	virtual void ProcessPossess() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
#if WITH_EDITOR
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
#endif

protected:
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact|Debug")
	bool bDebugMode = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact|Debug")
	bool bDebugCollisionBoxComponent = false;
#endif

private:
	virtual void SetupLoadedAsset(UObject* InOwner, UObject* LoadedObject) override;

	void BuildInteractableCollisionComponents();

	bool CreateBoxComponent(class USceneComponent* Root, const int32& Index);

	UFUNCTION()
	void OnBoxComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnBoxComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool ShowWidget(TSubclassOf<UUserWidget> WidgetTemplate = nullptr);
	bool RemoveWidget();
	void CheckSenseTouch(class AActor* Actor, bool On);
	void CheckSenseVisual(class AActor* Actor, bool On);

	bool SetActorUIImage(UObject* Object);
	bool SetCollisionComponentSetup(UObject* Object);

	virtual void Interact_Implementation(class AController* InstigatorController) override;
	virtual void StopInteracting_Implementation(class AController* InstigatorController) override;

	const bool CheckLimit() const;

	UFUNCTION()
	void UnlockTemporaryRegisterLock();

	virtual void ShapeUpdated_Implementation() override;
private:
	UPROPERTY(Transient)
	TArray<class UBoxComponent*> InteractableComponents;
	UPROPERTY(Transient)
	class UUserWidget* InteractWidgetInstance = nullptr;
	UPROPERTY(Transient)
	TSoftObjectPtr<class UTexture2D> InteractUIImage = nullptr;
	UPROPERTY(Transient)
	TArray<class AActor*> InteractableActors;
	UPROPERTY(Transient)
	class AActor* CurrentInteractingActor;
	UPROPERTY(Transient)
	bool bHasLimit;
	UPROPERTY(Transient)
	int32 Limit;
	UPROPERTY(Transient)
	TArray<bool> TurnedCollisionOnElement;
	UPROPERTY(Transient)
	TSoftClassPtr<class UUserWidget> WidgetTemplate;
	UPROPERTY(Transient)
	bool bTemporaryTickLockForRegister = false;

#if WITH_EDITORONLY_DATA
	UPROPERTY(Transient)
	bool bHasDebuggedCollisionBoxComponent = false;
#endif

	virtual TArray<FInformationSet> GetSettingsVariableSets_Implementation() const override;
};
