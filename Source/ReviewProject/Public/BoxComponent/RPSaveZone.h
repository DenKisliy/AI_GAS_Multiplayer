// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BoxComponent/RPBaseBoxComponent.h"

#include "../Characters/RPPlayerCharacter.h"
#include "GameFramework/GameMode.h"
#include "Kismet/GameplayStatics.h"

#include "RPSaveZone.generated.h"

/**
 * 
 */
UCLASS()
class REVIEWPROJECT_API ARPSaveZone : public ARPBaseBoxComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = GAS)
	TSubclassOf<UGameplayEffect> RecoverAttributeEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<AActor> ClassToFind;

public:
	// Sets default values for this actor's properties
	ARPSaveZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnUpdatedComponentOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnUpdatedComponentOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void UpdateCharacterSaveZoneValue(AActor* Other, bool Value);

	void CharacterInSaveZone();

	bool IsPointInCube(FVector Point);
};
