// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "../RPBotController.h"
#include "../../Characters/RPPlayerCharacter.h"

#include "RPBTTaskSetNextPoint.generated.h"

/**
 * 
 */
UCLASS()
class REVIEWPROJECT_API URPBTTaskSetNextPoint : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
