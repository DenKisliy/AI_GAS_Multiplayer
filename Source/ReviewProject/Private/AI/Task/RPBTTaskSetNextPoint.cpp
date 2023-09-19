// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/RPBTTaskSetNextPoint.h"

EBTNodeResult::Type URPBTTaskSetNextPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController)
	{
		ARPBotController* botController = Cast<ARPBotController>(AIController);
		if (botController)
		{
			UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
			
			if (blackboard->GetValueAsBool(FName("bFindActor")))
			{
				return EBTNodeResult::Failed;
			}

			if (!blackboard->GetValueAsBool(FName("bGoToNearestPatrolPoint")))
			{
				TArray<FVector> patrolPointsArray = botController->GetPatrolPoints();

				int PointID = blackboard->GetValueAsInt(FName("PointID"));

				if (!blackboard->GetValueAsBool(FName("bFindFirstPoint")))
				{
					PointID = patrolPointsArray.Num() == PointID + 1 ? 0 : PointID + 1;
				}
				else
				{
					blackboard->SetValueAsBool(FName("bFindFirstPoint"), false);
				}

				blackboard->SetValueAsVector(FName("NextPoint"), patrolPointsArray[PointID]);
				blackboard->SetValueAsInt(FName("PointID"), PointID);

				return EBTNodeResult::Succeeded;
			}
			else
			{
				blackboard->SetValueAsBool(FName("bGoToNearestPatrolPoint"), false);
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