// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/RPAcceleratingAbility.h"

URPAcceleratingAbility::URPAcceleratingAbility()
{
	AbilityInputID = EGDAbilityInputID::Acceleration;
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Acceleration")));
}

void URPAcceleratingAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ARPPlayerCharacter* Character = Cast<ARPPlayerCharacter>(ActorInfo->AvatarActor.Get());

	if (Character)
	{

		float currentStamina = Character->GetStaminaAttributeValue();

		if (currentStamina >= StaminaCost)
		{
			Character->SetStaminaAttributeValue(currentStamina - StaminaCost);

			Character->ActivateAcceleration(SpeedIncreaseFactor, TimeOfDeactivate);
			AddWaitTask();
		}
	}
}

void URPAcceleratingAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

bool URPAcceleratingAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	ARPPlayerCharacter* Character = Cast<ARPPlayerCharacter>(ActorInfo->AvatarActor.Get());

	if (Character)
	{
		if (!Character->bCanUseAbility)
			return false;
	}

	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	return true;
}

void URPAcceleratingAbility::AddWaitTask()
{
	UAbilityTask_WaitDelay* WaitTask = UAbilityTask_WaitDelay::WaitDelay(this, TimeOfDeactivate);
	FScriptDelegate Delegate;
	Delegate.BindUFunction(this, "DisableAbility");
	WaitTask->OnFinish.Add(Delegate);
	WaitTask->Activate();
}

void URPAcceleratingAbility::DisableAbility()
{
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
}