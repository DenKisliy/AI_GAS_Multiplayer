// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffActor/RPBuffActor.h"

// Sets default values
ARPBuffActor::ARPBuffActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	ParticleComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	SetReplicates(true);
	SetReplicateMovement(true);
}

void ARPBuffActor::ActivateBuff(EBuffAbilityID BuffID, int TimeToDeactivate)
{
	switch (BuffID)
	{
	case EBuffAbilityID::Acceleration:
		ParticleComponent->SetTemplate(AccelerationEffect);
		break;
	case EBuffAbilityID::Shield:
		ParticleComponent->SetTemplate(ShieldEffect);
		break;
	}

	if (BuffID != EBuffAbilityID::None)
	{
		ActivBuff = BuffID;
		GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &ARPBuffActor::OnDestroy, TimeToDeactivate, false);
	}
}

void ARPBuffActor::OnDestroy()
{
	GetWorld()->GetTimerManager().ClearTimer(DestroyTimerHandle);
	OnDisableBuff.Broadcast(ActivBuff);
	Destroy();
}
