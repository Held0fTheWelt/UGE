#include "Core_AbilitySystem.h"
#include "Components/CoreAbilitySystemComponent.h"
#include "CoreAbilitySet.h"
#include "Interfaces/AbilitySystemComponentClassRegistry.h"
#include "PawnDataAbilitySetApplier.h"

DEFINE_LOG_CATEGORY(Core_AbilitySystem);

#define LOCTEXT_NAMESPACE "FCore_AbilitySystem"

void FCore_AbilitySystem::StartupModule()
{
	CoreAbilitySystemComponentClassRegistry::SetAbilitySystemComponentClass(UCoreAbilitySystemComponent::StaticClass());

	PawnDataAbilitySetApplier::SetApplier([](UAbilitySystemComponent* ASC, const TArray<UObject*>& AbilitySets, UObject* SourceObject)
	{
		if (UCoreAbilitySystemComponent* CoreASC = Cast<UCoreAbilitySystemComponent>(ASC))
		{
			FCoreAbilitySet_GrantedHandles Handles;
			for (UObject* Obj : AbilitySets)
			{
				if (UCoreAbilitySet* Set = Cast<UCoreAbilitySet>(Obj))
				{
					Set->GiveToAbilitySystem(CoreASC, &Handles, SourceObject);
				}
			}
		}
	});

	UE_LOG(Core_AbilitySystem, Display, TEXT("Core_AbilitySystem module has been loaded"));
}

void FCore_AbilitySystem::ShutdownModule()
{
	CoreAbilitySystemComponentClassRegistry::SetAbilitySystemComponentClass(nullptr);
	UE_LOG(Core_AbilitySystem, Display, TEXT("Core_AbilitySystem module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCore_AbilitySystem, Core_AbilitySystem)