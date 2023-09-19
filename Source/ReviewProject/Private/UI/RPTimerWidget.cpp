// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RPTimerWidget.h"

void URPTimerWidget::AddElementsInWidget()
{
	TimerTextBlock = NewObject<UTextBlock>(this, UTextBlock::StaticClass());

	SetTextForTimer(0);
	TimerTextBlock->SetColorAndOpacity(FSlateColor(FLinearColor::White));

	TimerHorizontalBox->AddChild(TimerTextBlock);
}

void URPTimerWidget::SetTextForTimer(int time)
{
	FString timerText = "00:00";

	if (time > 0)
	{
		int minutes = time / 60;
		FString minutesText = minutes > 9 ? FString::FromInt(minutes) : "0" + FString::FromInt(minutes);

		int second = time % 60;
		FString secondText = second > 9 ? FString::FromInt(second) : "0" + FString::FromInt(second);

		timerText = minutesText + ":" + secondText;
	}

	TimerTextBlock->SetText(FText::FromString(timerText));
}

void URPTimerWidget::SetTextForWinOrLostGame(bool bWin)
{
	TimerTextBlock->SetText(bWin ? FText::FromString("You win!") : FText::FromString("You lost!"));
	TimerTextBlock->SetColorAndOpacity(bWin ? FSlateColor(FLinearColor::Green) : FSlateColor(FLinearColor::Red));
}
