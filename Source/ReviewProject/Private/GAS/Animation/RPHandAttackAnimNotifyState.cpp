// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Animation/RPHandAttackAnimNotifyState.h"

void URPHandAttackAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	bHitByCharacter = false;
}

void URPHandAttackAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(MeshComp->GetOwner());

	FHitResult HitArrayLeft;
	FHitResult HitArrayRight;

	bool leftArm = UKismetSystemLibrary::SphereTraceSingle(MeshComp, MeshComp->GetSocketLocation("fist_l_collision"), MeshComp->GetSocketLocation("thumb_01_l"),
		12.f, UEngineTypes::ConvertToTraceType(ECC_Camera), false, ActorsToIgnore, EDrawDebugTrace::None, HitArrayLeft, true, FLinearColor::Gray, FLinearColor::Blue, 1.f);

	bool rightArm = UKismetSystemLibrary::SphereTraceSingle(MeshComp, MeshComp->GetSocketLocation("fist_r_collision"), MeshComp->GetSocketLocation("thumb_01_r"),
		12.f, UEngineTypes::ConvertToTraceType(ECC_Camera), false, ActorsToIgnore, EDrawDebugTrace::None, HitArrayRight, true, FLinearColor::Gray, FLinearColor::Blue, 1.f);

	if (leftArm || rightArm)
	{
		AActor* actor = leftArm ? HitArrayLeft.GetActor() : HitArrayRight.GetActor();
		ARPBaseCharacter* characterWhichDamaged = Cast<ARPBaseCharacter>(actor);
		ARPBaseCharacter* playerWhichAttack = Cast<ARPBaseCharacter>(MeshComp->GetOwner());

		if (characterWhichDamaged && playerWhichAttack && !bHitByCharacter)
		{
			bHitByCharacter = true;

			playerWhichAttack->PlayDamageGameplayEffect(EGDAbilityInputID::HandAttack, characterWhichDamaged);
		}
	}
}

void URPHandAttackAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	bHitByCharacter = false;
}