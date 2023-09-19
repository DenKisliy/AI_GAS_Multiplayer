// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/VerticalBox.h"
#include "Components/CanvasPanel.h"
#include "RPAttributeWidget.h"

#include "Kismet/GameplayStatics.h"

#include "RPPlayerAttributesWidget.generated.h"

/**
 * 
 */
class URPAttributeWidget;

UCLASS()
class REVIEWPROJECT_API URPPlayerAttributesWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<URPAttributeWidget> AttributeWidget;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* AttributesCanvasPanel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* VerticalBoxElement;

	URPAttributeWidget* HealthAttribute;
	
	URPAttributeWidget* ManaAttribute;
	
	URPAttributeWidget* StaminaAttribute;


public:
	UFUNCTION(BlueprintCallable)
	void AddAttributesInWidgets();
};
