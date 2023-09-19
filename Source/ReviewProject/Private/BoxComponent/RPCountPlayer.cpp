// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxComponent/RPCountPlayer.h"

void ARPCountPlayer::BeginPlay()
{
	Super::BeginPlay();
	IsPlayerInBoxComponent();
}

void ARPCountPlayer::OnUpdatedComponentOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IsPlayerInBoxComponent();
}

void ARPCountPlayer::IsPlayerInBoxComponent()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, FoundActors);

	ARPGameState* const RPGameState = GetWorld() != NULL ? GetWorld()->GetGameState<ARPGameState>() : NULL;
	if (RPGameState)
	{
		if (FoundActors.Num() == RPGameState->PlayerCountOfConnection)
		{
			for (int i = 0; i < FoundActors.Num(); i++)
			{
				ARPPlayerCharacter* foundCharacter = Cast<ARPPlayerCharacter>(FoundActors[i]);
				if (foundCharacter)
				{
					AController* controller = foundCharacter->GetController();
					if (controller)
					{
						APlayerController* playerController = Cast<APlayerController>(controller);
						if (playerController)
						{
							ARPMainPlayerHUD* hud = Cast<ARPMainPlayerHUD>(playerController->GetHUD());
							if (hud)
							{
								hud->SetTimeForTimerWidget(RPGameState->BeginPlayTimerCount);
								hud->ShowPlayerHUD(true);
							}
						}
					}
				}
			}
			RPGameState->StartBeginPlayTimerFunction();

			Destroy();
		}
	}
}
