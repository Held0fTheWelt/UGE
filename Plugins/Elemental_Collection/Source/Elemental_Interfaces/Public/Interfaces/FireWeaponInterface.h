// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FireWeaponInterface.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBulletSpeedUpdated, float, Speed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSocketLocationUpdated, FVector, SocketLocation);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFireWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ELEMENTAL_INTERFACES_API IFireWeaponInterface
{
	GENERATED_BODY()

public:
	/** Must be implemented by inheriting class */
	virtual FOnBulletSpeedUpdated& GetBulletSpeedUpdated() = 0;
	virtual FOnSocketLocationUpdated& GetSocketLocationUpdated() = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapons")
	float GetProjectileSpeed() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapons")
	FVector GetRelativeSocketLocation() const;
};
