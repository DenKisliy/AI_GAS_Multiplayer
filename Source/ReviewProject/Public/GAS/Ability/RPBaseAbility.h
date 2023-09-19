// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"

#include "../Attribute/RPAttributeSet.h"
#include "../../../ReviewProject.h"

#include "RPBaseAbility.generated.h"

class UGameplayEffect;

/**
 * 
 */
UCLASS()
class REVIEWPROJECT_API URPBaseAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	URPBaseAbility() {};

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Ability)
	EGDAbilityInputID AbilityInputID = EGDAbilityInputID::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass;

	virtual TSubclassOf<UGameplayEffect> GetDamageGameplayEffect();
};
