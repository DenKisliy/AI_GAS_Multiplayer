// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "RPPlayerAttributesWidget.h"
#include "RPTimerWidget.h"
#include "RPResultGameWidget.h"
#include "Kismet/GameplayStatics.h"
#include "RPResultGameWidget.h"

#include "RPPlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class REVIEWPROJECT_API URPPlayerHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<URPPlayerAttributesWidget> AttributesWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<URPTimerWidget> TimerWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<URPResultGameWidget> ResultGameWidget;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* TimerCanvasPanel;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* AttributesCanvasPanel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* PlayerCanvasPanel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* GameResultCanvasPanel;

	class URPPlayerAttributesWidget* PlayerAttributesWidget;

	class URPTimerWidget* PlayerTimerWidget;

	class URPResultGameWidget* GameResultWidget;

public:
	UFUNCTION(BlueprintCallable)
	void AddElementsInWidget();

	void ShowTimerWidget(bool Value);

	void ShowResultGame(bool GameResult);
};
