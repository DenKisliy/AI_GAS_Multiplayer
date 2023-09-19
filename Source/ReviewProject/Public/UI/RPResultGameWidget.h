// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/EditableText.h"
#include "Components/CanvasPanel.h"
#include "Components/SizeBox.h"
#include "Components/ScaleBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/EditableTextBox.h"
#include "Components/CanvasPanel.h"
#include "Blueprint/WidgetTree.h"

#include "RPResultGameWidget.generated.h"

/**
 * 
 */
UCLASS()
class REVIEWPROJECT_API URPResultGameWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* ResultGameCanvasPanel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* ResultGameVerticalBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UHorizontalBox* ResultGameHorizontalBox;

	class UTextBlock* ResultGameTextBlock;

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable)
	void ShowResultGame(bool bWin);
};
