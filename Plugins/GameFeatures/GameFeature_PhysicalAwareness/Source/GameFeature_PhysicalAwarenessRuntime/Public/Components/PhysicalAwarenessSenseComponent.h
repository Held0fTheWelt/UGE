// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnDrivingActorComponent_Base.h"
#include "Interfaces/PhysicalSenseInterface.h"
#include "PhysicalAwarenessSenseComponent.generated.h"



/**
 * 
 */
UCLASS()
class GAMEFEATURE_PHYSICALAWARENESSRUNTIME_API UPhysicalAwarenessSenseComponent : public UPawnDrivingActorComponent_Base,
	public IPhysicalSenseInterface
{
	GENERATED_BODY()
	
public:
	UPhysicalAwarenessSenseComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void BeginPlay() override;

private:
	virtual bool CanTouchActor_Implementation(const class AActor* Actor) const override;
	virtual bool CanTouchCurrentActor_Implementation() const override;
	virtual class AActor* GetCurrentTouchActor_Implementation() const override;
	virtual TArray<TSoftObjectPtr<AActor>> GetTouchableActors_Implementation() const override;
	virtual bool IsTouchingCurrentActor_Implementation() const override;
	virtual void SetTouchingCurrentActor_Implementation(const class AActor* Actor) override;
	
	virtual void SetupLoadedAsset(UObject* InOwner, UObject* LoadedObject) override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void SenseTouchActor(class AActor* Actor, bool On);
	void RegisterObject(class AActor* ActorToRegister, ESenseType Type);
	void ForgetObject(class AActor* ActorToForget, ESenseType Type);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SetCollisionComponentActive(bool bActive);
	class UPrimitiveComponent* GetCollisionComponent() const;
private:
	UPROPERTY(Transient)
	bool bDoesTrackActorsForGameManagement = false;
	UPROPERTY(Transient)
	bool bCanSenseActors = false;
	UPROPERTY(Transient)
	bool bCanTouchActors = false;

	UPROPERTY(Transient)
	TArray<TSoftObjectPtr<AActor>> TouchableActors = TArray<TSoftObjectPtr<AActor>>();
	UPROPERTY(Transient)
	class AActor* CurrentTouchedActor = nullptr;

#pragma region "CoreInformation Editor"
private:
	virtual TArray<FInformationSet> GetSettingsVariableSets_Implementation() const override;
#pragma region "CoreInformation Editor"
};
