// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#include "Structs/VerbMessageTypes.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(VerbMessageTypes)

FString FVerbMessage::ToString() const
{
	FString HumanReadableMessage;
	FVerbMessage::StaticStruct()->ExportText(/*out*/ HumanReadableMessage, this, /*Defaults=*/ nullptr, /*OwnerObject=*/ nullptr, PPF_None, /*ExportRootScope=*/ nullptr);
	return HumanReadableMessage;
}
