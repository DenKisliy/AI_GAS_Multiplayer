// Fill out your copyright notice in the Description page of Project Settings.


#include "Station/RPStationActor.h"

// Sets default values
ARPStationActor::ARPStationActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
    RootComponent = CollisionComponent;
    CollisionComponent->InitSphereRadius(200.0f);
	CollisionComponent->CanCharacterStepUpOn = ECB_No;
	CollisionComponent->SetShouldUpdatePhysicsVolume(true);
	CollisionComponent->SetCanEverAffectNavigation(false);
	CollisionComponent->bDynamicObstacle = true;

    // Create and position a mesh component so we can see where our sphere is
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
    Mesh->SetupAttachment(RootComponent);
    Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
    Mesh->SetWorldScale3D(FVector(0.8f));

    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ARPStationActor::OnUpdatedComponentOverlapBegin);
    CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ARPStationActor::OnUpdatedComponentOverlapEnd);

	SetReplicates(true);
}

// Called when the game starts or when spawned
void ARPStationActor::BeginPlay()
{
	Super::BeginPlay();

	ARPGameState* const gameState = GetWorld() != NULL ? GetWorld()->GetGameState<ARPGameState>() : NULL;

	if (gameState)
	{
		gameState->OnCaptureStationFinish.AddDynamic(this, &ARPStationActor::StationCapture);
	}
}

void ARPStationActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CountPlayerOnStation != CaptorPlayer.Num())
	{
		IsPlayerInSphere();
	}
}

void ARPStationActor::OnUpdatedComponentOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UpdateCaptureTimer(Other, true);
}

void ARPStationActor::OnUpdatedComponentOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UpdateCaptureTimer(Other, false);
}

void ARPStationActor::UpdateCaptureTimer(AActor* Other, bool Value)
{
	ARPPlayerCharacter* character = Cast<ARPPlayerCharacter>(Other);

	if (character)
	{
		if (Value)
		{
			CaptorPlayer.Add(character);
			CountPlayerOnStation = CaptorPlayer.Num();
			if (CountPlayerOnStation > 0)
			{
				if (CountPlayerOnStation == 1)
				{
					ActivateCaptureTimer();
				}
				else
				{
					AcceleratonCaptureTimer();
				}
			}
		}
		else
		{
			CaptorPlayer.Remove(character);
			CountPlayerOnStation = CaptorPlayer.Num();
			if (CountPlayerOnStation == 0)
			{
				DeactivateCaptureTimer();
			}
			else
			{
				AcceleratonCaptureTimer();
			}
		}

	}
}

void ARPStationActor::ActivateCaptureTimer()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, FoundActors);

	ARPGameState* const gameState = GetWorld() != NULL ? GetWorld()->GetGameState<ARPGameState>() : NULL;

	if (gameState)
	{
		int TimerCounter = gameState->GetCaptureStationTimerCount();
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
							hud->SetTimeForTimerWidget(TimerCounter);
						}
					}
				}
			}
		}
		gameState->CaptureStationTimerFunction();
	}
}

void ARPStationActor::AcceleratonCaptureTimer()
{
	ARPGameState* const gameState = GetWorld() != NULL ? GetWorld()->GetGameState<ARPGameState>() : NULL;

	if (gameState)
	{
		float timer = 1;
		if (CountPlayerOnStation > 1)
		{
			timer = (CountPlayerOnStation - 1) * 2;
		}

		gameState->AcceleratonCaptureStationTimer(timer);
	}
}

void ARPStationActor::DeactivateCaptureTimer()
{
	ARPGameState* const gameState = GetWorld() != NULL ? GetWorld()->GetGameState<ARPGameState>() : NULL;

	if (gameState)
	{
		gameState->DeactivateCaptureStationTimer();
	}
}

void ARPStationActor::StationCapture_Implementation()
{
	FActorSpawnParameters SpawnParams;

	GetWorld()->SpawnActor<AActor>(SpawnActor, GetActorLocation(), GetActorRotation(), SpawnParams);

	Destroy();
}

void ARPStationActor::IsPlayerInSphere()
{
	int ActorInSphere = 0;
	for (int i = 0; i < CaptorPlayer.Num(); i++)
	{
		if (FVector::Dist(GetActorLocation(), CaptorPlayer[i]->GetActorLocation()) <= CollisionComponent->GetScaledSphereRadius())
		{
			ActorInSphere = ActorInSphere + 1;
		}
	}

	if (ActorInSphere < CountPlayerOnStation)
	{
		CountPlayerOnStation = ActorInSphere;
		if (CountPlayerOnStation == 0)
		{
			DeactivateCaptureTimer();
		}
		else if (CountPlayerOnStation == 1)
		{
			ActivateCaptureTimer();
		}
		else
		{
			AcceleratonCaptureTimer();
		}
	}
}

