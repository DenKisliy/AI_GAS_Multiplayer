// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/RPBTTaskFindNearestPoint.h"

EBTNodeResult::Type URPBTTaskFindNearestPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (AIController)
	{
		BotController = Cast<ARPBotController>(AIController);

		if (BotController)
		{
			Blackboard = OwnerComp.GetBlackboardComponent();
			
			APawn* AIPawn = AIController->GetPawn();

			if (AIPawn)
			{
				BotCharacter = Cast<ARPAICharacter>(AIPawn);

				if (BotCharacter)
				{
					FindNearestPointFromPatrolPoints();
					return EBTNodeResult::Succeeded;
				}
				else
				{
					return EBTNodeResult::Failed;
				}
			}
			else
			{
				return EBTNodeResult::Failed;
			}
		}
		else
		{
			return EBTNodeResult::Failed;
		}
	}
	else
	{
		return EBTNodeResult::Failed;
	}
}

void URPBTTaskFindNearestPoint::FindNearestPointFromPatrolPoints()
{
	if (Blackboard->GetValueAsBool(FName("bGoToNearestPatrolPoint")))
	{
		FVector botLocation = BotCharacter->GetActorLocation();
		int PointID = 0;
		float minPointDist = -1;

		TArray<FVector> patrolPointsArray = BotController->GetPatrolPoints();

		for (int i = 0; i < patrolPointsArray.Num(); i++)
		{
			if (!IsPathWithoutHit(botLocation, patrolPointsArray[i]))
			{
				if (minPointDist > FVector::Dist(botLocation, patrolPointsArray[i]) || minPointDist == -1)
				{
					minPointDist = FVector::Dist(botLocation, patrolPointsArray[i]);
					PointID = i;
				}
			}
		}

		Blackboard->SetValueAsVector(FName("NextPoint"), patrolPointsArray[PointID]);
		Blackboard->SetValueAsInt(FName("PointID"), PointID);
		Blackboard->SetValueAsBool(FName("bFindFirstPoint"), true);
		Blackboard->SetValueAsBool(FName("bGoToNearestPatrolPoint"), false);
	}
}

bool URPBTTaskFindNearestPoint::IsPathWithoutHit(FVector ActorLocaton, FVector NextLocation)
{
	FHitResult Hit;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(BotCharacter);

	GetWorld()->LineTraceSingleByChannel(Hit, ActorLocaton, NextLocation, ECC_WorldStatic, QueryParams);

	return (Hit.bBlockingHit && IsValid(Hit.GetActor()));
}