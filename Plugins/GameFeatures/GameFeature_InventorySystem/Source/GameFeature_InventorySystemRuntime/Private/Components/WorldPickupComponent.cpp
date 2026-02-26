#include "Components/WorldPickupComponent.h"

#include "InventoryManagerComponent.h"
#include "InventoryItemDefinition.h"
#include "Structs/InteractionOptionBuilder.h"
#include "Structs/InteractionQuery.h"
#include "GameFramework/Pawn.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(WorldPickupComponent)

UWorldPickupComponent::UWorldPickupComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GameFeatureName = "GameFeature_InventorySystem";
	bLoadPrimaryAssets = false;
	bComponentHasActorAssetToLoad = false;
	bComponentHasComponentAssetToLoad = false;
}

// ---------------------------------------------------------------------------

void UWorldPickupComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UWorldPickupComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// ---------------------------------------------------------------------------

void UWorldPickupComponent::GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& OptionBuilder)
{
	OptionBuilder.AddInteractionOption(InteractionOption);
}

// ---------------------------------------------------------------------------

void UWorldPickupComponent::OnPickedUp(AActor* Instigator)
{
	if (!GetOwner() || !GetOwner()->HasAuthority()) { return; }
	if (!ItemDefinition) { return; }

	if (APawn* InstigatorPawn = Cast<APawn>(Instigator))
	{
		if (UInventoryManagerComponent* Inventory = InstigatorPawn->FindComponentByClass<UInventoryManagerComponent>())
		{
			Inventory->AddItemDefinitionTyped(ItemDefinition, StackCount);
		}
	}

	if (bDestroyOnPickup)
	{
		GetOwner()->Destroy();
	}
}
