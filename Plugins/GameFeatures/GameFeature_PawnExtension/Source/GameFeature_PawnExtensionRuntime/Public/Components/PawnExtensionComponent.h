// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "Interfaces/PawnExtensionComponentInterface.h"

#include "PawnExtensionComponent.generated.h"

namespace EEndPlayReason { enum Type : int; }

class UGameFrameworkComponentManager;
class UAbilitySystemComponentInterface;
class UObject;
struct FActorInitStateChangedParams;
struct FFrame;
struct FGameplayTag;

/**
 * Component that adds functionality to all Pawn classes so it can be used for characters/vehicles/etc.
 * This coordinates the initialization of other components.
 * Implements IPawnExtensionComponentInterface (Elemental) so Core and other plugins can use the interface.
 * Adapted from ULyraPawnExtensionComponent.
 */
UCLASS(MinimalAPI)
class UPawnExtensionComponent : public UPawnComponent, public IGameFrameworkInitStateInterface, public IPawnExtensionComponentInterface
{
	GENERATED_BODY()

public:

	GAMEFEATURE_PAWNEXTENSIONRUNTIME_API UPawnExtensionComponent(const FObjectInitializer& ObjectInitializer);

	/** The name of this overall feature, this one depends on the other named component features */
	static GAMEFEATURE_PAWNEXTENSIONRUNTIME_API const FName NAME_ActorFeatureName;

	//~ Begin IGameFrameworkInitStateInterface interface
	virtual FName GetFeatureName() const override { return NAME_ActorFeatureName; }
	GAMEFEATURE_PAWNEXTENSIONRUNTIME_API virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	GAMEFEATURE_PAWNEXTENSIONRUNTIME_API virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) override;
	GAMEFEATURE_PAWNEXTENSIONRUNTIME_API virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) override;
	GAMEFEATURE_PAWNEXTENSIONRUNTIME_API virtual void CheckDefaultInitialization() override;
	//~ End IGameFrameworkInitStateInterface interface

	/** Returns the pawn extension component if one exists on the specified actor. */
	UFUNCTION(BlueprintPure, Category = "WarCollection|Pawn")
	static GAMEFEATURE_PAWNEXTENSIONRUNTIME_API UPawnExtensionComponent* FindPawnExtensionComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UPawnExtensionComponent>() : nullptr); }

	/** Gets the pawn data, which is used to specify pawn properties in data */
	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }

	/** Sets the current pawn data */
	GAMEFEATURE_PAWNEXTENSIONRUNTIME_API void SetPawnData(const UObject* InPawnData);

	//~IPawnExtensionComponentInterface (Elemental)
	UAbilitySystemComponent* GetAbilitySystemComponent_Implementation() const override;
	void SetPawnData_Implementation(UObject* InPawnData) override;
	void InitializeAbilitySystem_Implementation(UAbilitySystemComponent* ASC, UObject* Owner) override;
	void UninitializeAbilitySystem_Implementation() override;
	void HandleControllerChanged_Implementation() override;
	void HandlePlayerStateReplicated_Implementation() override;

	/** Gets the current ability system component as script interface (internal use). Use GetAbilitySystemComponent via interface for UAbilitySystemComponent*. */
	TScriptInterface<UAbilitySystemComponentInterface> GetAbilitySystemComponentScriptInterface() const { return AbilitySystemComponent; }

	/** Should be called by the owning pawn to become the avatar of the ability system. */
	GAMEFEATURE_PAWNEXTENSIONRUNTIME_API void InitializeAbilitySystem(TScriptInterface<UAbilitySystemComponentInterface> InASC, AActor* InOwnerActor);

	/** Should be called by the owning pawn to remove itself as the avatar of the ability system. */
	GAMEFEATURE_PAWNEXTENSIONRUNTIME_API void UninitializeAbilitySystem();

	/** Should be called by the owning pawn when the pawn's controller changes. */
	GAMEFEATURE_PAWNEXTENSIONRUNTIME_API void HandleControllerChanged();

	/** Should be called by the owning pawn when the player state has been replicated. */
	GAMEFEATURE_PAWNEXTENSIONRUNTIME_API void HandlePlayerStateReplicated();

	/** Should be called by the owning pawn when the input component is setup. */
	GAMEFEATURE_PAWNEXTENSIONRUNTIME_API void SetupPlayerInputComponent();

	/** Register with the OnAbilitySystemInitialized delegate and broadcast if our pawn has been registered with the ability system component */
	GAMEFEATURE_PAWNEXTENSIONRUNTIME_API void OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate Delegate);

	/** Register with the OnAbilitySystemUninitialized delegate fired when our pawn is removed as the ability system's avatar actor */
	GAMEFEATURE_PAWNEXTENSIONRUNTIME_API void OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate Delegate);

protected:

	GAMEFEATURE_PAWNEXTENSIONRUNTIME_API virtual void OnRegister() override;
	GAMEFEATURE_PAWNEXTENSIONRUNTIME_API virtual void BeginPlay() override;
	GAMEFEATURE_PAWNEXTENSIONRUNTIME_API virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	GAMEFEATURE_PAWNEXTENSIONRUNTIME_API void OnRep_PawnData();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Delegate fired when our pawn becomes the ability system's avatar actor */
	FSimpleMulticastDelegate OnAbilitySystemInitialized;

	/** Delegate fired when our pawn is removed as the ability system's avatar actor */
	FSimpleMulticastDelegate OnAbilitySystemUninitialized;

	/** Pawn data used to create the pawn. Specified from a spawn function or on a placed instance. */
	UPROPERTY(EditInstanceOnly, ReplicatedUsing = OnRep_PawnData, Category = "WarCollection|Pawn")
	TObjectPtr<const UObject> PawnData;

	/** Pointer to the ability system component that is cached for convenience. */
	UPROPERTY(Transient)
	TScriptInterface<UAbilitySystemComponentInterface> AbilitySystemComponent;
};
