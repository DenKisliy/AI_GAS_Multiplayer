// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"

#include "../Characters/RPPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "../HUD/RPMainPlayerHUD.h"
#include "../Spawn/SpawnElementsActor.h"

#include "RPGameState.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCaptureStationFinish);

UENUM(BlueprintType)
enum class ETimerType : uint8
{
	None,
	BeginPlay,
	CaptureStation,
};

UCLASS()
class REVIEWPROJECT_API ARPGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(ReplicatedUsing = OnRep_TimeChecker)
	int TimeChecker;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FCaptureStationFinish OnCaptureStationFinish;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Find Actors")
	TSubclassOf<AActor> PlayerActorToFind;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Find Actors")
	TSubclassOf<AActor> SpawnActorAfterStationCapture;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Timers")
	int BeginPlayTimerCount = 10;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Timers")
	int CaptureStationTimerCount = 90;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Session")
	int PlayerCountOfConnection = 3;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Win conditions")
	int CountCaptureStationToWin = 3;

private:
	FTimerHandle StateTimerHandle;

	float TimerInterval = 0;
	
	int TimerCounter;

	ARPMainPlayerHUD* PlayerHUD;

	ETimerType TimerType = ETimerType::None;

	int CaptureStationCount = 0;

	int PlayerCount;

public:
	void StartBeginPlayTimerFunction();

	void CaptureStationTimerFunction();

	void AcceleratonCaptureStationTimer(float NewTimerInterval);

	void DeactivateCaptureStationTimer();

	UFUNCTION()
	void OnTimerCounterFunction();

	int GetCaptureStationTimerCount();

	int GetBeginPlayTimerCount();

	UFUNCTION(NetMulticast, Reliable)
	void SpawnActorsAfterStationCapture(int SpawnIndex);

	UFUNCTION()
	void OnActorDestroyed(ARPBaseCharacter* player);

protected:
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;

private:
	void SetCounter();

	UFUNCTION()
	void OnRep_TimeChecker();

	void SetPlayersCanUseAbilities();

	void ShowTextForGameEnd(bool bWin);
};
