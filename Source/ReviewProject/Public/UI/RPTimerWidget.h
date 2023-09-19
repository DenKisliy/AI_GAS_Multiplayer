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

#include "Kismet/GameplayStatics.h"

#include "RPTimerWidget.generated.h"

/**
 * 
 */
UCLASS()
class REVIEWPROJECT_API URPTimerWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* TimerCanvasPanel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* TimerVerticalBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UHorizontalBox* TimerHorizontalBox;

	class UTextBlock* TimerTextBlock;

public:
	void AddElementsInWidget();

	UFUNCTION()
	void SetTextForTimer(int time);

	UFUNCTION()
	void SetTextForWinOrLostGame(bool bWin);
	
};
