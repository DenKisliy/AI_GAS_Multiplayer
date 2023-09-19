// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/RPMainPlayerHUD.h"


void ARPMainPlayerHUD::BeginPlay()
{
	Super::BeginPlay();
	CreatePlayerWidget();
}

void ARPMainPlayerHUD::CreatePlayerWidget()
{
	if (PlayerHUD == NULL)
	{
		PlayerHUD = CreateWidget<URPPlayerHUD>(this->GetOwningPlayerController(), PlayerHUDWidget);
		PlayerHUD->AddElementsInWidget();


		SetDateForAttibutesWidget();

		SetTimeForTimerWidget(0);
	}
}

void ARPMainPlayerHUD::SetDateForAttibutesWidget()
{
	ARPPlayerCharacter* player = Cast<ARPPlayerCharacter>(GetOwningPawn());

	player->AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URPAttributeSet::GetHealthAttribute()).AddUObject(player, &ARPPlayerCharacter::OnHealthUpdated);

	player->AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URPAttributeSet::GetManaAttribute()).AddUObject(player, &ARPPlayerCharacter::OnManaUpdated);

	player->AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URPAttributeSet::GetStaminaAttribute()).AddUObject(player, &ARPPlayerCharacter::OnStaminaUpdated);

	URPAttributeWidget* healthAttribute = PlayerHUD->PlayerAttributesWidget->HealthAttribute;

	if (player->GetMaxHealthAttributeValue() != 0)
	{
		healthAttribute->SetMaxValueForAttribute(player->GetMaxHealthAttributeValue());
		healthAttribute->OnAttributeChangeValue(player->GetMaxHealthAttributeValue());
	}
	else if (player->GetHealthAttributeValue() != 0)
	{
		healthAttribute->SetMaxValueForAttribute(player->GetHealthAttributeValue());
		healthAttribute->OnAttributeChangeValue(player->GetHealthAttributeValue());
	}

	player->OnHealthChange.AddDynamic(healthAttribute, &URPAttributeWidget::OnAttributeChangeValue);


	URPAttributeWidget* manaAttribute = PlayerHUD->PlayerAttributesWidget->ManaAttribute;

	if (player->GetMaxManaAttributeValue() != 0)
	{
		manaAttribute->SetMaxValueForAttribute(player->GetMaxManaAttributeValue());
		manaAttribute->OnAttributeChangeValue(player->GetMaxManaAttributeValue());
	}
	else if (player->GetManaAttributeValue() != 0)
	{
		manaAttribute->SetMaxValueForAttribute(player->GetManaAttributeValue());
		manaAttribute->OnAttributeChangeValue(player->GetManaAttributeValue());
	}


	player->OnManaChange.AddDynamic(manaAttribute, &URPAttributeWidget::OnAttributeChangeValue);

	URPAttributeWidget* staminaAttribute = PlayerHUD->PlayerAttributesWidget->StaminaAttribute;

	if (player->GetMaxStaminaAttributeValue() != 0)
	{
		staminaAttribute->SetMaxValueForAttribute(player->GetMaxStaminaAttributeValue());
		staminaAttribute->OnAttributeChangeValue(player->GetMaxStaminaAttributeValue());
	}
	else if (player->GetStaminaAttributeValue() != 0)
	{
		staminaAttribute->SetMaxValueForAttribute(player->GetStaminaAttributeValue());
		staminaAttribute->OnAttributeChangeValue(player->GetStaminaAttributeValue());
	}

	player->OnStaminaChange.AddDynamic(staminaAttribute, &URPAttributeWidget::OnAttributeChangeValue);
}

void ARPMainPlayerHUD::ShowPlayerHUD(bool Value)
{
	if (Value)
	{
		CreatePlayerWidget();
		PlayerHUD->AddToViewport();
	}
	else
	{
		PlayerHUD->RemoveFromParent();
	}
}

void ARPMainPlayerHUD::SetTimeForTimerWidget(int Time)
{
	if (PlayerHUD)
	{
		PlayerHUD->ShowTimerWidget(Time != -1);

		PlayerHUD->PlayerTimerWidget->SetTextForTimer(Time);
	}
}

void ARPMainPlayerHUD::SetGameResultWin(bool bResult)
{
	PlayerHUD->ShowResultGame(bResult);
}
