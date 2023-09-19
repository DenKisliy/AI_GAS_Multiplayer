// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RPAICharacter.h"
#include "GameFramework/InputSettings.h"

void ARPAICharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ARPAICharacter::SetAIIndex(int32 Index)
{
	AIIndex = Index;
}

void ARPAICharacter::ActivateAbility(EGDAbilityInputID AbilityID)
{
	for (TSubclassOf<URPBaseAbility>& Ability : DefaultAbilities)
	{
		if (Ability)
		{
			if (Ability.GetDefaultObject()->AbilityInputID == AbilityID)
			{
				GetAbilitySystemComponent()->TryActivateAbilitiesByTag(Ability.GetDefaultObject()->AbilityTags);
			}
		}
	}
}