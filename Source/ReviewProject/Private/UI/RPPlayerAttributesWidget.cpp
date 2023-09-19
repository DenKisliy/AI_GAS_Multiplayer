// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RPPlayerAttributesWidget.h"

void URPPlayerAttributesWidget::AddAttributesInWidgets()
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	HealthAttribute = CreateWidget<URPAttributeWidget>(this, AttributeWidget);
	HealthAttribute->CreateElementsInAttributeWidget("Health:", FLinearColor::Green);
	VerticalBoxElement->AddChild(HealthAttribute);

	ManaAttribute = CreateWidget<URPAttributeWidget>(this, AttributeWidget);
	ManaAttribute->CreateElementsInAttributeWidget("Mana:", FLinearColor::Blue);

	VerticalBoxElement->AddChild(ManaAttribute);

	StaminaAttribute = CreateWidget<URPAttributeWidget>(this, AttributeWidget);
	StaminaAttribute->CreateElementsInAttributeWidget("Stamina:", FLinearColor::Yellow);

	VerticalBoxElement->AddChild(StaminaAttribute);
}
