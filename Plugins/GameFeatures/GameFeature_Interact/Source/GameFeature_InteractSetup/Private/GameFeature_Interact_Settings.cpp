#include "GameFeature_Interact_Settings.h"

UGameFeature_Interact_Settings::UGameFeature_Interact_Settings(const FObjectInitializer& obj)
	: Super(obj)
{
	bUseGlobalQueryCollisionDebug = false;
	bGlobalQueryCollisionDebug = false;
	bUseGlobalDefaultInteractLimit = false;
	bHasDefaultInteractLimit = false;
	DefaultInteractLimit = 1;
	DefinedCollisionDebug = {};
}
