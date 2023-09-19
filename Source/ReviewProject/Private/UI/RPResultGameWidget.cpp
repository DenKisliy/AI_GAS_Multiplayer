// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RPResultGameWidget.h"

void URPResultGameWidget::NativeConstruct()
{
}

void URPResultGameWidget::ShowResultGame(bool bWin)
{
	ResultGameTextBlock = NewObject<UTextBlock>(this, UTextBlock::StaticClass());

	ResultGameTextBlock->SetText(bWin ? FText::FromString("You win!") : FText::FromString("You lost!"));
	ResultGameTextBlock->SetColorAndOpacity(bWin ? FSlateColor(FLinearColor::Green) : FSlateColor(FLinearColor::Red));

	ResultGameHorizontalBox->AddChild(ResultGameTextBlock);
}
