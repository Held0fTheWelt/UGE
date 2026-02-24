// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/DynamicMeshComponent.h"
#include "Interfaces/NoTouchCollisionInterface.h"
#include "NoTouchDynamicMeshComponent.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENTAL_CLASSES_API UNoTouchDynamicMeshComponent : public UDynamicMeshComponent, public INoTouchCollisionInterface
{
	GENERATED_BODY()
	
public:
	UNoTouchDynamicMeshComponent(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
	{
		UE_LOG(LogTemp, Verbose, TEXT("UNoTouchDynamicMeshComponent constructed (ImplementsInterface: %d)"),
			GetClass()->ImplementsInterface(UNoTouchCollisionInterface::StaticClass()));
	}
};
