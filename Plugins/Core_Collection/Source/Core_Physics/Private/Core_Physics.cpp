// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core_Physics.h"
#include "Interfaces/InteractionCollisionChannelProvider.h"

DEFINE_LOG_CATEGORY(LogCore_Physics);

class FCore_PhysicsModule : public IModuleInterface
{
public:
	virtual void StartupModule() override
	{
		InteractionCollisionChannelRegistry::Set(ECC_GameTraceChannel1);
	}

	virtual void ShutdownModule() override
	{
		InteractionCollisionChannelRegistry::Set(ECC_WorldDynamic);
	}
};

IMPLEMENT_MODULE(FCore_PhysicsModule, Core_Physics)
