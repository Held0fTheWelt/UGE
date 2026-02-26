#pragma once

#include "CoreMinimal.h"
#include "Components/CombinedBundleActorComponent.h"
#include "Interfaces/IInteractableTarget.h"
#include "IPickupable.h"
#include "Structs/InteractionOption.h"
#include "WorldPickupComponent.generated.h"

class UInventoryItemDefinition;
struct FInteractionQuery;
struct FInteractionOptionBuilder;

/**
 * UWorldPickupComponent
 *
 * Attach to any actor to make it a world-space pickup.
 * Implements IInteractableTarget (exposes a pickup interaction option to the player)
 * and IPickupable (grants the configured item to the instigator's inventory on pickup).
 *
 * Follows the UCombinedBundleActorComponent pattern — no actor subclass required.
 * Added to actors via GameFeature setup, identical to all other bundle components.
 *
 * Replaces ALyraWorldCollectable (ShooterCore).
 */
UCLASS(ClassGroup = (Inventory), meta = (BlueprintSpawnableComponent))
class GAMEFEATURE_INVENTORYSYSTEMRUNTIME_API UWorldPickupComponent
	: public UCombinedBundleActorComponent
	, public IInteractableTarget
	, public IPickupable
{
	GENERATED_BODY()

public:
	UWorldPickupComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~IInteractableTarget
	virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& OptionBuilder) override;
	//~End IInteractableTarget

	//~IPickupable
	virtual void OnPickedUp(AActor* Instigator) override;
	//~End IPickupable

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Item to add to the instigator's inventory on pickup. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup")
	TSubclassOf<UInventoryItemDefinition> ItemDefinition;

	/** Number of items to grant per pickup. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup", meta = (ClampMin = 1))
	int32 StackCount = 1;

	/** Whether to destroy the owner actor after a successful pickup. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup")
	bool bDestroyOnPickup = true;

	/**
	 * Interaction option presented to the player.
	 * Configure Text, InteractionAbilityToGrant, and InteractionWidgetClass here.
	 * InteractableTarget is set automatically by the interaction system.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup|Interaction")
	FInteractionOption InteractionOption;
};
