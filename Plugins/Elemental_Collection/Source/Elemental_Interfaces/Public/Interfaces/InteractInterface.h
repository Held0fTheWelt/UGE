/*
 * Copyright (c) 2024 Yves Tanas
 * All Rights Reserved.
 *
 * This file is part of the Collections project.
 *
 * Unauthorized copying of this file, via any medium, is strictly prohibited.
 * Proprietary and confidential.
 *
 * This software may be used only as expressly authorized by the copyright holder.
 * Unless required by applicable law or agreed to in writing, software distributed
 * under this license is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.
 *
 * For licensing inquiries, please contact: yves.tanas@example.com
 *
 * Contributors:
 *   Yves Tanas <yves.tanas@example.com>
 *
 * -------------------------------------------------------------------------------
 * File:        [InteractInterface.h]
 * Created:     [2025-06-12]
 * Description: [Short file/module purpose]
 * -------------------------------------------------------------------------------
 */
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractWithPawn, class AActor*, Interactor, class AController*, InteractorInstigator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStopInteractingWithPawn, class AActor*, Interactor, class AController*, InteractorInstigator);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ELEMENTAL_INTERFACES_API IInteractInterface
{
	GENERATED_BODY()

public:
	virtual FOnInteractWithPawn& GetOnInteractWithPawn() = 0;
	virtual FOnStopInteractingWithPawn& GetOnStopInteractingWithPawn() = 0;


	///** Called when the interactable is interacted with */
	///** @param Interactor The actor that is interacting with the interactable */
	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
	//bool Interact(AActor* Interactor);
	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
	//bool IsInteracting(AActor* Interactor);
	///** Called when the interactable is stopped from interacting with */
	///** @param Interactor The actor that is interacting with the interactable */
	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
	//bool StopInteract(AActor* Interactor);
	///** Called when the interactable is focused */
	///** @param Interactor The actor that is interacting with the interactable */
	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
	//bool Focus(AActor* Interactor);
	///** Called when the interactable is unfocused */
	///** @param Interactor The actor that is interacting with the interactable */
	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
	//bool Unfocus(AActor* Interactor);
	///** Returns true if the interactable can be interacted with */
	///** @param Interactor The actor that is interacting with the interactable */
	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
	//bool CanInteract(AActor* Interactor) const;
};
