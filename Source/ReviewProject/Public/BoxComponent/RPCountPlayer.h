// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BoxComponent/RPBaseBoxComponent.h"

#include "Kismet/GameplayStatics.h"
#include "../GameState/RPGameState.h"
#include "GameFramework/PlayerController.h"

#include "RPCountPlayer.generated.h"

/**
 * 
 */
UCLASS()
class REVIEWPROJECT_API ARPCountPlayer : public ARPBaseBoxComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<AActor> ClassToFind;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnUpdatedComponentOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void IsPlayerInBoxComponent();
};
