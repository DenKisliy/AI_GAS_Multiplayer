// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/RPBaseAbility.h"

#include "../../Characters/RPBaseCharacter.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/Actor.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Animation/AnimMontage.h"
#include "GameplayEffect.h"

#include "RPHandAttackAbility.generated.h"

/**
 * 
 */

class UAnimMontage;

USTRUCT(BlueprintType)
struct FAttackStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Probability;
};

UCLASS()
class REVIEWPROJECT_API URPHandAttackAbility : public URPBaseAbility
{
	GENERATED_BODY()
	
public:
	URPHandAttackAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

private:
	FName GetRandomAttack();

	void PlayAttackMontage(ARPBaseCharacter* Player);

	void AddWaitTask(float WaitTime);

	UFUNCTION()
	void DisableAbility();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UAnimMontage* PunchMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TArray<FAttackStruct> ArrayOfAttack;
};
