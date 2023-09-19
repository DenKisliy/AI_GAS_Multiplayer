// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/RPGameState.h"

void ARPGameState::StartBeginPlayTimerFunction()
{
	if (BeginPlayTimerCount != -1 && TimerType == ETimerType::None)
	{
		TimerCounter = BeginPlayTimerCount;
		TimerInterval = 1;
		TimerType = ETimerType::BeginPlay;
		GetWorld()->GetTimerManager().SetTimer(StateTimerHandle, this, &ARPGameState::OnTimerCounterFunction, TimerInterval, true);
	}
}

void ARPGameState::CaptureStationTimerFunction()
{
	if (CaptureStationTimerCount != -1 && TimerType == ETimerType::None)
	{
		TimerCounter = CaptureStationTimerCount;
		TimerInterval = 1;
		TimerType = ETimerType::CaptureStation;
		GetWorld()->GetTimerManager().SetTimer(StateTimerHandle, this, &ARPGameState::OnTimerCounterFunction, TimerInterval, true);
	}
}

void ARPGameState::AcceleratonCaptureStationTimer(float NewTimerInterval)
{
	if (TimerCounter > 0)
	{
		TimerInterval = 1 / NewTimerInterval;
		GetWorld()->GetTimerManager().ClearTimer(StateTimerHandle);

		GetWorld()->GetTimerManager().SetTimer(StateTimerHandle, this, &ARPGameState::OnTimerCounterFunction, TimerInterval, true);
	}
}

void ARPGameState::DeactivateCaptureStationTimer()
{
	TimerCounter = -1;

	TimerType = ETimerType::None;

	SetCounter();
}

void ARPGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARPGameState, TimeChecker);
}

void ARPGameState::OnRep_TimeChecker()
{
	if (TimerCounter != TimeChecker)
	{
		TimerCounter = TimeChecker;
		
		GetWorld()->GetTimerManager().ClearTimer(StateTimerHandle);

		GetWorld()->GetTimerManager().SetTimer(StateTimerHandle, this, &ARPGameState::OnTimerCounterFunction, TimerInterval, true);

		SetCounter();
	}
}

void ARPGameState::OnTimerCounterFunction()
{
	TimerCounter = TimerCounter - 1;

	if (HasAuthority())
	{
		if (TimerCounter % 10 == 9)
		{
			TimeChecker = TimerCounter;
		}
	}

	SetCounter();
}

void ARPGameState::SetCounter()
{
	if (!UKismetSystemLibrary::IsValid(PlayerHUD))
	{
		APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (playerController)
		{
			AHUD* hud = playerController->GetHUD();
			
			if (UKismetSystemLibrary::IsValid(hud))
			{
				ARPMainPlayerHUD* playerHUD = Cast<ARPMainPlayerHUD>(hud);
				if (playerHUD)
				{
					PlayerHUD = playerHUD;
				}
			}
		}
	}

	PlayerHUD->SetTimeForTimerWidget(TimerCounter);

	if (TimerCounter == -1)
	{
		GetWorld()->GetTimerManager().ClearTimer(StateTimerHandle);

		switch (TimerType)
		{
		case ETimerType::BeginPlay:
			PlayerCount = PlayerCountOfConnection;
			SpawnActorsAfterStationCapture(1);
			SetPlayersCanUseAbilities();
			break;
		case ETimerType::CaptureStation:
			CaptureStationCount = CaptureStationCount + 1;
			if (CaptureStationCount == CountCaptureStationToWin)
			{
				TArray<AActor*> FoundActors;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARPAICharacter::StaticClass(), FoundActors);
				for (AActor* ActorFound : FoundActors)
				{
					ActorFound->Destroy();
				}

				ShowTextForGameEnd(true);
			}
			else
			{
				OnCaptureStationFinish.Broadcast();
				SpawnActorsAfterStationCapture(CaptureStationCount + 1);
			}
			break;
		default:
			break;
		}

		TimerType = ETimerType::None;
	}
}

int ARPGameState::GetCaptureStationTimerCount()
{
	return CaptureStationTimerCount;
}

int ARPGameState::GetBeginPlayTimerCount()
{
	return BeginPlayTimerCount;
}

void ARPGameState::SetPlayersCanUseAbilities()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), PlayerActorToFind, FoundActors);

	for (int i = 0; i < FoundActors.Num(); i++)
	{
		ARPPlayerCharacter* foundCharacter = Cast<ARPPlayerCharacter>(FoundActors[i]);
		if (foundCharacter)
		{
			foundCharacter->bCanUseAbility = true;
			foundCharacter->OnActorDestroed.AddDynamic(this, &ARPGameState::OnActorDestroyed);
		}
	}
}

void ARPGameState::SpawnActorsAfterStationCapture_Implementation(int SpawnIndex)
{
	if (HasAuthority())
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), SpawnActorAfterStationCapture, FoundActors);

		for (int i = 0; i < FoundActors.Num(); i++)
		{
			ASpawnElementsActor* foundCharacter = Cast<ASpawnElementsActor>(FoundActors[i]);
			if (foundCharacter)
			{
				if (foundCharacter->SpawnGroup == SpawnIndex)
				{
					foundCharacter->StartSpawn();
				}
			}
		}
	}
}

void ARPGameState::OnActorDestroyed(ARPBaseCharacter* player)
{
	PlayerCount = PlayerCount - 1;

	if (PlayerCount == 0)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARPAICharacter::StaticClass(), FoundActors);
		for (AActor* ActorFound : FoundActors)
		{
			ActorFound->Destroy();
		}

		ShowTextForGameEnd(false);
	}
}

void ARPGameState::ShowTextForGameEnd(bool bWin)
{
	if (HasAuthority())
	{
		if (bWin)
		{
			GetWorld()->ServerTravel(TEXT("/Game/ThirdPersonCPP/Maps/WinGame?listen"));
		}
		else
		{
			GetWorld()->ServerTravel(TEXT("/Game/ThirdPersonCPP/Maps/LostGame?listen"));
		}
	}
}