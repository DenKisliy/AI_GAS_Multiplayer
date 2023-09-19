// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "../AI/RPBotController.h"

#include "SpawnElementsActor.generated.h"

UCLASS()
class REVIEWPROJECT_API ASpawnElementsActor : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Spawn")
	TSubclassOf<AActor> SpawnActor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Spawn")
	int SpawnGroup = 1;

public:	
	// Sets default values for this actor's properties
	ASpawnElementsActor();

	void StartSpawn();
};
