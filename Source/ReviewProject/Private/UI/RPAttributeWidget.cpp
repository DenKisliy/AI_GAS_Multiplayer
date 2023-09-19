// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RPAttributeWidget.h"

void URPAttributeWidget::CreateElementsInAttributeWidget(FString AttributeName, FLinearColor AttriuteColor)
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UTextBlock* textAttributeName = NewObject<UTextBlock>(this, UTextBlock::StaticClass());

	textAttributeName->SetText(FText::FromString(AttributeName));
	textAttributeName->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	
	USizeBox* sizeBoxForTextAttributeName = NewObject<USizeBox >(this, USizeBox::StaticClass());
	sizeBoxForTextAttributeName->AddChild(textAttributeName);
	sizeBoxForTextAttributeName->SetWidthOverride(200.0);

	HorizontalBoxElement->AddChild(sizeBoxForTextAttributeName);

	ValueAttribute = NewObject<UTextBlock>(this, UTextBlock::StaticClass());
	ValueAttribute->SetColorAndOpacity(FSlateColor(AttriuteColor));

	USizeBox* sizeBoxForProgressBarAttribute = NewObject<USizeBox>(this, USizeBox::StaticClass());
	sizeBoxForProgressBarAttribute->AddChild(ValueAttribute);
	sizeBoxForProgressBarAttribute->SetWidthOverride(200.0);

	HorizontalBoxElement->AddChild(sizeBoxForProgressBarAttribute);

	ValueAttributeColor = AttriuteColor;
}

void URPAttributeWidget::OnAttributeChangeValue(float AttributePerception)
{
	ValueAttribute->SetText(FText::FromString(""));
	FString text = FString::SanitizeFloat(AttributePerception) + "/" + FString::SanitizeFloat(GetMaxValueForAttribute());
	ValueAttribute->SetText(FText::FromString(text));

	ValueAttribute->SetColorAndOpacity((AttributePerception / GetMaxValueForAttribute()) < 0.2 ? FLinearColor::Red : ValueAttributeColor);
}

void URPAttributeWidget::SetMaxValueForAttribute(float Value)
{
	MaxValueForAttribute = Value;
}

float URPAttributeWidget::GetMaxValueForAttribute()
{
	return MaxValueForAttribute;
}
