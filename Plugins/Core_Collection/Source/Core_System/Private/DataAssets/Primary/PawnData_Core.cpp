#include "DataAssets/Primary/PawnData_Core.h"
#include "PawnDataAbilitySetApplier.h"

#include "AbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PawnData_Core)

UPawnData_Core::UPawnData_Core(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TArray<UObject*> UPawnData_Core::GetAbilitySetsToGrant() const
{
	TArray<UObject*> Result;
	for (UObject* Obj : AbilitySets)
	{
		if (Obj) { Result.Add(Obj); }
	}
	return Result;
}

void UPawnData_Core::GrantAbilitySetsToAbilitySystem(UAbilitySystemComponent* ASC, UObject* SourceObject)
{
	PawnDataAbilitySetApplier::Invoke(ASC, GetAbilitySetsToGrant(), SourceObject);
}
