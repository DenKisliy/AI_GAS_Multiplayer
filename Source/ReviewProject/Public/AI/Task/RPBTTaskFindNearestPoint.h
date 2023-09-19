// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "../RPBotController.h"

#include "RPBTTaskFindNearestPoint.generated.h"

/**
 * 
 */
UCLASS()
class REVIEWPROJECT_API URPBTTaskFindNearestPoint : public UBTTaskNode
{
	GENERATED_BODY()

private:
	ARPAICharacter* BotCharacter;

	ARPBotController* BotController;

	UBlackboardComponent* Blackboard;

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	void FindNearestPointFromPatrolPoints();

	bool IsPathWithoutHit(FVector ActorLocaton, FVector NextLocation);
	
};
