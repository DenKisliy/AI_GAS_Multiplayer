// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/RPFireBallAbility.h"

URPFireBallAbility::URPFireBallAbility()
{
	AbilityInputID = EGDAbilityInputID::FireBall;
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.FireBall")));
}

void URPFireBallAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ARPBaseCharacter* Character = Cast<ARPBaseCharacter>(ActorInfo->AvatarActor.Get());

	if (Character)
	{
		float currentMana = Character->GetManaAttributeValue();

		if (currentMana >= ManaCost)
		{
			Character->SetManaAttributeValue(currentMana - ManaCost);

			SpawnFireBall(ActorInfo);
			FireBallActor->StartMove(TimeOfDestroy, ActorInfo->AvatarActor.Get());
		}
	}
}

void URPFireBallAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

bool URPFireBallAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
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

void URPFireBallAbility::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (ActorInfo != nullptr && ActorInfo->AvatarActor != nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("URPFireBallAbility::InputReleased")));
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
	}
}

void URPFireBallAbility::SpawnFireBall(const FGameplayAbilityActorInfo* ActorInfo)
{
	FActorSpawnParameters SpawnParams;
	ARPBaseCharacter* player = Cast<ARPBaseCharacter>(ActorInfo->AvatarActor.Get());

	if (player)
	{
		FireBallActor = GetWorld()->SpawnActor<ARPFireBall>(SpawnActor, player->GetSpawnFireBallArrowLocation(), player->GetActorRotation(), SpawnParams);
	}
}