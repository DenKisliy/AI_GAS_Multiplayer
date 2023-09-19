// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/RPShieldAbility.h"

URPShieldAbility::URPShieldAbility()
{
	AbilityInputID = EGDAbilityInputID::Shield;
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Shield")));
}

void URPShieldAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ARPPlayerCharacter* Character = Cast<ARPPlayerCharacter>(ActorInfo->AvatarActor.Get());

	if (Character)
	{
		float currentMana = Character->GetManaAttributeValue();

		if (currentMana >= ManaCost)
		{
			Character->SetManaAttributeValue(currentMana - ManaCost);

			Character->ActivateVisualShield(TimeOfDeactivate);
			AddWaitTask();
		}
	}
}

void URPShieldAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

bool URPShieldAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
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

void URPShieldAbility::AddWaitTask()
{
	UAbilityTask_WaitDelay* WaitTask = UAbilityTask_WaitDelay::WaitDelay(this, TimeOfDeactivate);
	FScriptDelegate Delegate;
	Delegate.BindUFunction(this, "DisableAbility");
	WaitTask->OnFinish.Add(Delegate);
	WaitTask->Activate();
}

void URPShieldAbility::DisableAbility()
{
	ARPPlayerCharacter* Character = Cast<ARPPlayerCharacter>(GetCurrentActorInfo()->AvatarActor.Get());

	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
}