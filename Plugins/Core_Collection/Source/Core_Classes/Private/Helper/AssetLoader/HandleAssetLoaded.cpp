#include "Helper/AssetLoader/HandleAssetLoaded.h"

#include "DataAssets/Primary/InformativePrimaryDataAsset.h"

#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"


DEFINE_LOG_CATEGORY_STATIC(Log_CombinedHandleAssetLoaded, Log, All);

int32 UHandleAssetLoaded::GetIndexOfObject(UObject* LoadedObject, const TArray<FName>& InputArray)
{
	if (LoadedObject == nullptr)
	{
		UE_LOGFMT(Log_CombinedHandleAssetLoaded, Error, "{Time}: {Line} {Class}: Loaded object is null",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return INDEX_NONE;
	}
	else
	{
		UE_LOGFMT(Log_CombinedHandleAssetLoaded, VeryVerbose, "{Time}: {Line} {Class}: Loaded object: {Object}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Object", LoadedObject->GetFName()));
	}

	UInformativePrimaryDataAsset* InformativeAsset = Cast<UInformativePrimaryDataAsset>(LoadedObject);
	if (!InformativeAsset)
	{
		UE_LOGFMT(Log_CombinedHandleAssetLoaded, Error, "{Time}: {Line} {Class}: Loaded object is not a UInformativePrimaryDataAsset",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return INDEX_NONE;
	}

	int32 PrimaryAssetIndex  = InputArray.Find(InformativeAsset->AssetTypeName);

	if (PrimaryAssetIndex == INDEX_NONE)
	{
		UE_LOGFMT(Log_CombinedHandleAssetLoaded, Error, "{Time}: {Line} {Class}: Loaded object {Object} is not in the list of PrimaryAssetNamesToLoad",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Object", LoadedObject->GetFName()));
		return INDEX_NONE;
	}
	return PrimaryAssetIndex;
}
