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
 * File:        RegisterComponentInterface.h
 * Created:     2025-06-12
 * Description: Interface for actors or objects that can register/unregister actor components,
 *              e.g. for dynamic systems, pooling, or runtime extension.
 * -------------------------------------------------------------------------------
 */
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RegisterComponentInterface.generated.h"

 /**
  * URegisterComponentInterface
  *
  * Unreal Engine interface base class for register/unregister component operations.
  */
UINTERFACE(MinimalAPI, Blueprintable)
class URegisterComponentInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * IRegisterComponentInterface
 *
 * Interface for any object that can register or unregister components dynamically
 * (for example: registries, managers, gameplay systems).
 */
class ELEMENTAL_INTERFACES_API IRegisterComponentInterface
{
	GENERATED_BODY()

public:
	/** Register a component with this interface instance.
	 *  @param Component	Pointer to the UActorComponent to register.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interfaces | Register")
	void RegisterComponentWithInterface(UActorComponent* Component);

	/** Unregister a previously registered component.
	 *  @param Component	Pointer to the UActorComponent to unregister.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interfaces | Register")
	void UnregisterComponentWithInterface(UActorComponent* Component);
};
