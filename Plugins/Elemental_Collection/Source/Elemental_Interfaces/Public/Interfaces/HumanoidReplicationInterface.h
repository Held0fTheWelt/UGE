// Copyright Epic Games, Inc. All Rights Reserved.
// Elemental_Interfaces: Replication update for humanoid characters.
// GameFeatures (e.g. HumanoidMovement) call this without depending on Core.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HumanoidReplicationInterface.generated.h"

UINTERFACE(MinimalAPI, BlueprintType, NotBlueprintable)
class UHumanoidReplicationInterface : public UInterface
{
	GENERATED_BODY()
};

/** Implemented by the project's character for optimized replication. HumanoidMovement and similar GameFeatures use only this interface. */
class ELEMENTAL_INTERFACES_API IHumanoidReplicationInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Humanoid|Replication")
	virtual bool UpdateSharedReplication() { return false; }

	/** Applies replicated acceleration (e.g. from FastSharedReplication). Engine's UCharacterMovementComponent::Acceleration is protected; implement in character to apply via own movement component if needed. */
	UFUNCTION(BlueprintCallable, Category = "Humanoid|Replication")
	virtual void SetReplicatedAcceleration(const FVector& InAcceleration) {}
};

// -------------------------------------------------------------------------
// IReplicatedAccelerationReceiver – implemented by movement components that can apply replicated acceleration
// -------------------------------------------------------------------------

UINTERFACE(MinimalAPI, BlueprintType, NotBlueprintable)
class UReplicatedAccelerationReceiver : public UInterface
{
	GENERATED_BODY()
};

/** Implemented by a UCharacterMovementComponent-derived class that can receive and apply replicated acceleration (Acceleration is protected in engine). Used by CoreCharacter::OnRep_ReplicatedAcceleration. */
class ELEMENTAL_INTERFACES_API IReplicatedAccelerationReceiver
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Humanoid|Replication")
	virtual void SetReplicatedAcceleration(const FVector& InAcceleration) = 0;
};
