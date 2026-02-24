// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class UClass;

/**
 * Central registry for the AbilitySystemComponent class.
 * Core_AbilitySystem sets the class at module startup; Core_Classes reads it when creating the ASC.
 * Allows Core_Classes to create the ASC without depending on Core_AbilitySystem.
 */
namespace CoreAbilitySystemComponentClassRegistry
{
	/** Returns the registered ASC class, or nullptr if none registered (caller may fall back to UAbilitySystemComponent::StaticClass()). */
	CORE_INTERFACES_API UClass* GetAbilitySystemComponentClass();

	/** Called by Core_AbilitySystem at module startup to register UCoreAbilitySystemComponent::StaticClass(). */
	CORE_INTERFACES_API void SetAbilitySystemComponentClass(UClass* InClass);
}
