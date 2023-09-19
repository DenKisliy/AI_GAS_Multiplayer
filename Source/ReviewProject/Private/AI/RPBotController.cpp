// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/RPBotController.h"

ARPBotController::ARPBotController(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("SenseSight");
	Sight->SightRadius = PerceptionRadius;
	Sight->LoseSightRadius = LosePerceptionRadius;
	Sight->PeripheralVisionAngleDegrees = 90;
	Sight->DetectionByAffiliation.bDetectNeutrals = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = true;
	Sight->DetectionByAffiliation.bDetectEnemies = true;

	Hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>("SenseHearing");
	Hearing->HearingRange = PerceptionRadius;
	Hearing->bUseLoSHearing = true;
	Hearing->LoSHearingRange = LosePerceptionRadius;
	Hearing->DetectionByAffiliation.bDetectNeutrals = true;
	Hearing->DetectionByAffiliation.bDetectFriendlies = true;
	Hearing->DetectionByAffiliation.bDetectEnemies = true;

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComponent");
	AIPerceptionComponent->ConfigureSense(*Sight);
	AIPerceptionComponent->ConfigureSense(*Hearing);
	AIPerceptionComponent->SetDominantSense(Sight->GetSenseImplementation());

	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	BlackboardComponent->InitializeBlackboard(*BlackboardBot);

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ARPBotController::OnPerception);
}


void ARPBotController::BeginPlay()
{
	Super::BeginPlay();
}

void ARPBotController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	StartPatroling();
}

//Get start data
void ARPBotController::SetDataInBlackboardBot()
{
	if (BlackboardBot)
	{
		GetDataFromDataTableWithAIPonts();
		BlackboardComponent->SetValueAsBool(FName("bGoToNearestPatrolPoint"), true);
	}
}

void ARPBotController::GetDataFromDataTableWithAIPonts()
{
	if (DataTableWithAIPonts)
	{
		PatrolPoints.Empty();
		FString ContextString;
		TArray<FName> RowNames;
		RowNames = DataTableWithAIPonts->GetRowNames();

		for (auto& name : RowNames)
		{
			FAIPointsForPatrolStruct* row = DataTableWithAIPonts->FindRow<FAIPointsForPatrolStruct>(name, ContextString);
			if (row)
			{
				if (row->AI_Index == Agent->GetAIIndex())
				{
					PatrolPoints.Add(row->Point);
				}
			}
		}

		if (!PatrolPoints.Num())
		{
			for (auto& name : RowNames)
			{
				FAIPointsForPatrolStruct* row = DataTableWithAIPonts->FindRow<FAIPointsForPatrolStruct>(name, ContextString);
				if (row)
				{
					PatrolPoints.Add(row->Point);
				}
			}
		}
	}
}

//call when detect player
void ARPBotController::OnPerception(AActor* Actor, FAIStimulus Stimulus)
{
	DetectPlayer = Cast<ARPPlayerCharacter>(Actor);

	if (DetectPlayer)
	{
		if (!DetectPlayer->IsCharacterInSafeArea())
		{
			BlackboardComponent->SetValueAsObject(FName("DetectPlayer"), DetectPlayer);

			StopMovement();

			BlackboardComponent->SetValueAsBool(FName("bFindActor"), true);
		}
	}
}

TArray<FVector> ARPBotController::GetPatrolPoints()
{
	return PatrolPoints;
}

void ARPBotController::StartPatroling()
{
	ARPAICharacter* Bot = Cast<ARPAICharacter>(GetPawn());
	if (Bot)
	{
		Agent = Bot;
	}

	if (BTBot)
	{
		RunBehaviorTree(BTBot);
	}

	SetDataInBlackboardBot();
}
