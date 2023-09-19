// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/RPBTTaskPlayerLocation.h"

EBTNodeResult::Type URPBTTaskPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	
	if (AIController)
	{
		ARPBotController* botController = Cast<ARPBotController>(AIController);
		
		if (botController)
		{
			UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

			UObject* detectObject = blackboard->GetValueAsObject(FName("DetectPlayer"));
			
			if (detectObject)
			{
				ARPPlayerCharacter* playerCharacter = Cast<ARPPlayerCharacter>(detectObject);

				if (playerCharacter)
				{
					if (playerCharacter->IsCharacterInSafeArea() && !blackboard->GetValueAsBool(FName("bGoToNearestPatrolPoint")))
					{
						blackboard->SetValueAsBool(FName("bGoToNearestPatrolPoint"), true);
						blackboard->SetValueAsBool(FName("bFindActor"), false);

						return EBTNodeResult::Failed;
					}
					else
					{
						botController->StopMovement();

						blackboard->SetValueAsVector(FName("NextPoint"), playerCharacter->GetActorLocation());

						return EBTNodeResult::Succeeded;
					}
				}
				else
				{
					blackboard->SetValueAsBool(FName("bGoToNearestPatrolPoint"), true);
					blackboard->SetValueAsBool(FName("bFindActor"), false);

					return EBTNodeResult::Failed;
				}
			}
			else
			{
				blackboard->SetValueAsBool(FName("bGoToNearestPatrolPoint"), true);
				blackboard->SetValueAsBool(FName("bFindActor"), false);
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