// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawn/SpawnElementsActor.h"

// Sets default values
ASpawnElementsActor::ASpawnElementsActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetReplicates(true);
}

// Called when the game starts or when spawned
void ASpawnElementsActor::StartSpawn()
{
	FActorSpawnParameters SpawnParams;

	AActor* spawnActor = GetWorld()->SpawnActor<AActor>(SpawnActor, GetActorLocation(), GetActorRotation(), SpawnParams);

	ARPAICharacter* bot = Cast<ARPAICharacter>(spawnActor);

	if (bot)
	{
		bot->SpawnDefaultController();
	}

	Destroy();
}

