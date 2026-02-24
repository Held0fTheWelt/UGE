#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DemoDisplayActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDemoColorChanged, const FLinearColor&, Color);

UCLASS()
class CORE_DEMO_API ADemoDisplayActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ADemoDisplayActor();

	FOnDemoColorChanged OnDemoColorChanged;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class USceneComponent* Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UTextRenderComponent* TextRender;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
	class UMaterialInstanceDynamic* Color;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void UpdateDisplayColor(const FLinearColor& InColor);
};
