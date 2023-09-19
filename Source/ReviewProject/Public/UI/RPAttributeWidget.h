// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/ProgressBar.h"
#include "Components/SizeBox.h"
#include "Styling/SlateColor.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/GameplayStatics.h"

#include "RPAttributeWidget.generated.h"

/**
 * 
 */
UCLASS()
class REVIEWPROJECT_API URPAttributeWidget : public UUserWidget
{
	GENERATED_BODY()	

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* AttributeCanvasPanel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UHorizontalBox* HorizontalBoxElement;

	class UTextBlock* ValueAttribute;

public:
	void CreateElementsInAttributeWidget(FString AttributeName, FLinearColor ProgressBarColor);

	UFUNCTION()
	void OnAttributeChangeValue(float AttributePerception);

	void SetMaxValueForAttribute(float Value);

	float GetMaxValueForAttribute();

private:
	FLinearColor ValueAttributeColor;

	float MaxValueForAttribute = 100;
};
