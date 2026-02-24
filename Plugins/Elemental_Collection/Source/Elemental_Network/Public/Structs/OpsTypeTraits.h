#pragma once

#include "CoreMinimal.h"
#include "Structs/SharedRepMovement.h"
#include "OpsTypeTraits.generated.h"

template<>
struct TStructOpsTypeTraits<FSharedRepMovement> : public TStructOpsTypeTraitsBase2<FSharedRepMovement>
{
	enum
	{
		WithNetSerializer = true,
		WithNetSharedSerialization = true,
	};
};