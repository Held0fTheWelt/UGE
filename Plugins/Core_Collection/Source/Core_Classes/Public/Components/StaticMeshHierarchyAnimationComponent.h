// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CombinedBundleActorComponent.h"
#include "Interfaces/DebugMassesAndCOMProviderInterface.h"
#include "StaticMeshHierarchyAnimationComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(Log_StaticMeshAnimator, Log, All);

/** Einzelnes Mesh: Name, Masse und berechnete COM in World-Space (für Abfrage/Blueprint). */
USTRUCT(BlueprintType, Category = "StaticMeshHierarchy|COM")
struct CORE_CLASSES_API FStaticMeshMassAndCOM
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly, Category = "COM") FName MeshName;
	UPROPERTY(BlueprintReadOnly, Category = "COM") float Mass = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "COM") FVector WorldCOM = FVector::ZeroVector;
};
/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CORE_CLASSES_API UStaticMeshHierarchyAnimationComponent : public UCombinedBundleActorComponent, public IDebugMassesAndCOMProviderInterface
{
	GENERATED_BODY()

public:
	UStaticMeshHierarchyAnimationComponent(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(Transient)
	TMap<FName, TObjectPtr<class USceneComponent>> StaticMeshComponents;
	UPROPERTY(Transient)
	bool bIsStaticMeshCollectionAvailable;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void SetupLoadedAsset(UObject* InOwner, UObject* LoadedObject) override;

	virtual TArray<FInformationSet> GetSettingsVariableSets_Implementation() const override;

private:
	void BindStaticMeshComposerFinished();
	/** Wenn true (z. B. via heli.DebugCOM 1): zeichnet pro Mesh Masse + COM und den Gesamt-COM. */
	void DrawDebugMassesAndCenterOfMass();

	UFUNCTION()
	void OnNotifyMeshSetupComplete(FMeshComponentCollection ComponentCollection);

public:
	virtual void SetDebugMassesAndCOMEnabled_Implementation(bool bEnabled) override;

	/** Berechnete Center-of-Mass des gesamten Mesh-Trees (massengewichtet). Gibt false zurück wenn keine Meshes oder keine Masse. */
	UFUNCTION(BlueprintCallable, Category = "StaticMeshHierarchy|COM")
	bool GetHierarchyCenterOfMassWorld(FVector& OutWorldCOM) const;

	/** Pro Mesh: Name, Masse und COM in World-Space. Leer wenn noch keine StaticMeshComponents geladen. */
	UFUNCTION(BlueprintCallable, Category = "StaticMeshHierarchy|COM")
	TArray<FStaticMeshMassAndCOM> GetPerMeshMassesAndCOMs() const;

	/** Override in subclasses to provide the physics body COM (e.g. from the simulating Body primitive). Returns false by default. */
	virtual bool GetPhysicsBodyCenterOfMassWorld(FVector& OutWorldCOM) const { return false; }
};
