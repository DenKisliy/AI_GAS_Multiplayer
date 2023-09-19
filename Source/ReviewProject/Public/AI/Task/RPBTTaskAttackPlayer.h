// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "../RPBotController.h"
#include "../../Characters/RPPlayerCharacter.h"
#include "Math/Vector.h"
#include "Kismet/KismetMathLibrary.h"

#include "RPBTTaskAttackPlayer.generated.h"

/**
 * 
 */
UCLASS()
class REVIEWPROJECT_API URPBTTaskAttackPlayer : public UBTTaskNode
{
	GENERATED_BODY()

private:
	ARPPlayerCharacter* PlayerCharacter;

	ARPAICharacter* BotCharacter;

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	EGDAbilityInputID CanActivateAbility();

	void RotateAndActivateAbility();
};
