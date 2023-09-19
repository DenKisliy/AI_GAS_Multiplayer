// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxComponent/RPBaseBoxComponent.h"

// Sets default values
ARPBaseBoxComponent::ARPBaseBoxComponent()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	BoxComponent->InitBoxExtent(FVector(100, 100, 100));
	BoxComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	BoxComponent->CanCharacterStepUpOn = ECB_No;
	BoxComponent->SetShouldUpdatePhysicsVolume(true);
	BoxComponent->SetCanEverAffectNavigation(false);
	BoxComponent->bDynamicObstacle = true;
	RootComponent = BoxComponent;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARPBaseBoxComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARPBaseBoxComponent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

