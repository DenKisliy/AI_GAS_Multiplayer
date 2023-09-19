// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicAttackActor/RPFireBall.h"

// Sets default values
ARPFireBall::ARPFireBall()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(15.0f);
	CollisionComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	CollisionComponent->CanCharacterStepUpOn = ECB_No;
	CollisionComponent->SetShouldUpdatePhysicsVolume(true);
	CollisionComponent->SetCanEverAffectNavigation(false);
	CollisionComponent->bDynamicObstacle = true;
	RootComponent = CollisionComponent;

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->SetUpdatedComponent(CollisionComponent);
	MovementComponent->InitialSpeed = 500.0f;
	MovementComponent->MaxSpeed = 600.0f;
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->bShouldBounce = true;
	MovementComponent->Bounciness = 0.3f;
	MovementComponent->ProjectileGravityScale = 0.0f;
	MovementComponent->bAutoActivate = false;

	FireBallParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireBallParticle"));
	FireBallParticleComponent->AttachToComponent(CollisionComponent, FAttachmentTransformRules::KeepRelativeTransform);

	CollisionComponent->OnComponentHit.AddDynamic(this, &ARPFireBall::OnHit);

	SetReplicates(true);
	SetReplicateMovement(true);
}

void ARPFireBall::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GetWorld()->GetTimerManager().ClearTimer(DestroyTimerHandle);

	ARPBaseCharacter* hitActor = Cast<ARPBaseCharacter>(OtherActor);
	ARPBaseCharacter* actorWhichActivate = Cast<ARPBaseCharacter>(ActorWhichActivateAbility);

	if (hitActor)
	{
		if (actorWhichActivate)
		{
			actorWhichActivate->PlayDamageGameplayEffect(EGDAbilityInputID::FireBall, hitActor);
		}
	}

	Destroy();
}

void ARPFireBall::OnDestroy()
{
	GetWorld()->GetTimerManager().ClearTimer(DestroyTimerHandle);

	Destroy();
}

void ARPFireBall::StartMove(int DestroyTime, AActor* ActivateActor)
{
	ActorWhichActivateAbility = ActivateActor;
	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &ARPFireBall::OnDestroy, DestroyTime, false);
	MovementComponent->Activate();
}

