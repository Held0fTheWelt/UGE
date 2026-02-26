#pragma once

#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"

#include "AccoladeDefinition.generated.h"

class USoundBase;
class UObject;

/**
 * FAccoladeDefinitionRow
 *
 * One row in a DataTable/DataRegistry-backed accolade catalogue.
 * Loaded by UAccoladeHostWidget when an accolade notification arrives.
 *
 * Migrated from FLyraAccoladeDefinitionRow (ShooterCore).
 */
USTRUCT(BlueprintType)
struct FAccoladeDefinitionRow : public FTableRowBase
{
	GENERATED_BODY()

	/** Text shown in the accolade toast. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DisplayName;

	/** Sound to play when the accolade fires. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<USoundBase> Sound;

	/** Icon shown in the toast (Texture, Material, or SlateTextureAtlasInterface). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayThumbnail = "true",
		AllowedClasses = "Texture,MaterialInterface,SlateTextureAtlasInterface",
		DisallowedClasses = "MediaTexture"))
	TSoftObjectPtr<UObject> Icon;

	/** How long (seconds) to display the toast. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DisplayDuration = 1.0f;

	/**
	 * Which widget instance should display this accolade.
	 * Matched against UAccoladeHostWidget::LocationName.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag LocationTag;

	/** Tags associated with this accolade (used for suppression logic). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer AccoladeTags;

	/**
	 * When this accolade is displayed, any currently pending accolades that have
	 * any of these tags will be cancelled.
	 * (e.g., a triple-elim suppresses an existing double-elim.)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer CancelAccoladesWithTag;
};

// ---------------------------------------------------------------------------

/**
 * UAccoladeDefinition
 *
 * Standalone DataAsset variant (no DataTable required).
 * Useful for one-off accolades defined directly as assets.
 *
 * Migrated from ULyraAccoladeDefinition (ShooterCore).
 */
UCLASS(BlueprintType)
class UAccoladeDefinition : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USoundBase> Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayThumbnail = "true",
		AllowedClasses = "Texture,MaterialInterface,SlateTextureAtlasInterface",
		DisallowedClasses = "MediaTexture"))
	TObjectPtr<UObject> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer AccoladeTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer CancelAccoladesWithTag;
};
