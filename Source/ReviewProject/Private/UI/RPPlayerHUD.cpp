// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RPPlayerHUD.h"

void URPPlayerHUD::AddElementsInWidget()
{
	PlayerTimerWidget = CreateWidget<URPTimerWidget>(this, TimerWidget);
	PlayerTimerWidget->AddElementsInWidget();
	TimerCanvasPanel->AddChild(PlayerTimerWidget);
	ShowTimerWidget(true);

	PlayerAttributesWidget = CreateWidget<URPPlayerAttributesWidget>(this, AttributesWidget);
	PlayerAttributesWidget->AddAttributesInWidgets();
	AttributesCanvasPanel->AddChild(PlayerAttributesWidget);
}

void URPPlayerHUD::ShowTimerWidget(bool Value)
{
	if (Value)
	{
		TimerCanvasPanel->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		TimerCanvasPanel->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void URPPlayerHUD::ShowResultGame(bool GameResult)
{
	PlayerAttributesWidget->RemoveFromParent();
	PlayerTimerWidget->RemoveFromParent();

	GameResultWidget = CreateWidget<URPResultGameWidget>(this, ResultGameWidget);
	GameResultWidget->ShowResultGame(GameResult);
	GameResultCanvasPanel->AddChild(GameResultWidget);
}
