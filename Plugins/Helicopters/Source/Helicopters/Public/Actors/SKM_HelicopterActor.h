// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/HelicopterActor.h"
#include "SKM_HelicopterActor.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(Log_HelicopterActor, Log, All);
/**
 * 
 */
UCLASS()
class HELICOPTERS_API ASKM_HelicopterActor : public AHelicopterActor
{
	GENERATED_BODY()
	
public:
	ASKM_HelicopterActor();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class USkeletalMeshComponent> Mesh;

	virtual void SetupLoadedAsset(UObject* InOwner, UObject* LoadedObject) override;
private:
	class USceneComponent* GetSceneComponent_Implementation(const FName& ComponentName) const override;
	void SetMeshAndAnimInstance(TSoftObjectPtr<class USkeletalMesh> Mesh, TSoftClassPtr<class UAnimInstance> AnimInstance);
};
