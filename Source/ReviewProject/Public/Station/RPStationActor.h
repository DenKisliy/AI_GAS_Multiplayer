// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../Characters/RPPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "../GameState/RPGameState.h"
#include "../HUD/RPMainPlayerHUD.h"
#include "GameFramework/PlayerController.h"
#include "Math/Vector.h"

#include "RPStationActor.generated.h"

UCLASS()
class REVIEWPROJECT_API ARPStationActor : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollisionComponent;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<AActor> ClassToFind;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TSubclassOf<AActor> SpawnActor;

private:
	int CountPlayerOnStation = 0;

	TArray<AActor*> CaptorPlayer;

public:	
	// Sets default values for this actor's properties
	ARPStationActor(); 
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(NetMulticast, Reliable)
	void StationCapture();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnUpdatedComponentOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnUpdatedComponentOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void UpdateCaptureTimer(AActor* Other, bool Value);

	void ActivateCaptureTimer();
	
	void AcceleratonCaptureTimer();
	
	void DeactivateCaptureTimer();

	void IsPlayerInSphere();
};
