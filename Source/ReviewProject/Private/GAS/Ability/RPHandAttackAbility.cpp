// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/RPHandAttackAbility.h"

URPHandAttackAbility::URPHandAttackAbility()
{
	AbilityInputID = EGDAbilityInputID::HandAttack;
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.HandAttack")));
}

void URPHandAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ARPBaseCharacter* Character = Cast<ARPBaseCharacter>(ActorInfo->AvatarActor.Get());

	if (Character && PunchMontage)
	{
		PlayAttackMontage(Character);
	}
}

void URPHandAttackAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

bool URPHandAttackAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
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

FName URPHandAttackAbility::GetRandomAttack()
{
	FName sectionName = NAME_None;

	TArray<int> test;

	if (ArrayOfAttack.Num() > 0)
	{
		int randomValue = rand() % 100 + 1;
		for (int i = 0; i < ArrayOfAttack.Num(); i++)
		{
			test.Add(test.Num() > 0 ? (test.Last() + ArrayOfAttack[i].Probability) : ArrayOfAttack[i].Probability);
		}

		for (int i = 0; i < test.Num(); i++)
		{
			if (randomValue < test[i] + 1)
			{
				sectionName = ArrayOfAttack[i].Name;
				break;
			}
		}
	}

	return sectionName;
}

void URPHandAttackAbility::PlayAttackMontage(ARPBaseCharacter* Player)
{
	UAnimInstance* AnimInstance = (Player->GetMesh()) ? Player->GetMesh()->GetAnimInstance() : nullptr;

	if (AnimInstance)
	{
		FName sectionName = GetRandomAttack();

		if (sectionName != NAME_None)
		{
			float timeStempStart = 0;
			float timeStempEnd = 0;

			PunchMontage->GetSectionStartAndEndTime(PunchMontage->GetSectionIndex(sectionName), timeStempStart, timeStempEnd);
			AnimInstance->Montage_JumpToSection(sectionName, PunchMontage);

			Player->MulticastPlayMontage(PunchMontage, sectionName);
			AddWaitTask(timeStempEnd - timeStempStart);
		}
		else
		{
			DisableAbility();
		}
	}
}

void URPHandAttackAbility::AddWaitTask(float WaitTime)
{
	UAbilityTask_WaitDelay* WaitTask = UAbilityTask_WaitDelay::WaitDelay(this, WaitTime);
	FScriptDelegate Delegate;
	Delegate.BindUFunction(this, "DisableAbility");
	WaitTask->OnFinish.Add(Delegate);
	WaitTask->Activate();
}

void URPHandAttackAbility::DisableAbility()
{
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
}