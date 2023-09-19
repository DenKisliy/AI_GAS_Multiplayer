// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attribute/RPAttributeSet.h"


void URPAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(URPAttributeSet, Health, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPAttributeSet, Mana, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPAttributeSet, Stamina, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPAttributeSet, MaxHealth, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPAttributeSet, MaxMana, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPAttributeSet, MaxStamina, COND_OwnerOnly, REPNOTIFY_Always);
}

void URPAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		if (GetMaxHealth() != 0)
		{
			SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
		}
	}

	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		if (GetMaxMana() != 0)
		{
			SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));
		}
	}

	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		if (GetMaxStamina() != 0)
		{
			SetStamina(FMath::Clamp(GetStamina(), 0.0f, GetMaxStamina()));
		}
	}
}

void URPAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPAttributeSet, Health, OldHealth);
}

void URPAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPAttributeSet, Mana, OldMana);
}

void URPAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPAttributeSet, Stamina, OldStamina);
}

void URPAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPAttributeSet, MaxHealth, OldMaxHealth);
}

void URPAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPAttributeSet, MaxMana, OldMaxMana);
}

void URPAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPAttributeSet, MaxStamina, OldMaxStamina);
}