// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GiftPlacingActor.generated.h"

UCLASS()
class CORE_EXTRAS_API AGiftPlacingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AGiftPlacingActor();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	TObjectPtr<class USceneComponent> Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	TObjectPtr<class UStaticMeshComponent> GiftMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	TObjectPtr<class UBoxComponent> BoxCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	TObjectPtr<class UStaticMeshComponent> SpawnLocationMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	TObjectPtr<class USceneComponent> DecoyPosition_1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	TObjectPtr<class USceneComponent> DecoyPosition_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	TObjectPtr<class USceneComponent> DecoyPosition_3;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	TObjectPtr<class USceneComponent> DecoyPosition_4;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	TObjectPtr<class USceneComponent> DecoyPosition_5;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	TObjectPtr<class USceneComponent> DecoyPosition_6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gift Actor")
	TSoftClassPtr<class AActor> GiftActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Solve")
	TSoftObjectPtr<class USoundBase> OpenedSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Solve")
	TSoftObjectPtr<class USoundBase> EffectsSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Solve")
	TSoftObjectPtr<class UNiagaraSystem> DecoyEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Solve")
	TSoftClassPtr<class UUserWidget> SolveInformationWidget;
protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
};
