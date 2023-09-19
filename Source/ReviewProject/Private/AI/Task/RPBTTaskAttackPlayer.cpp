// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/RPBTTaskAttackPlayer.h"


EBTNodeResult::Type URPBTTaskAttackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerComp.GetAIOwner()->GetPawn();

	AAIController* AIController = OwnerComp.GetAIOwner();

	if (AIController)
	{
		APawn* AIPawn = AIController->GetPawn();
		
		if(AIPawn)
		{
			BotCharacter = Cast<ARPAICharacter>(AIPawn);
			
			if (BotCharacter)
			{
				UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

				UObject* detectObject = blackboard->GetValueAsObject(FName("DetectPlayer"));

				if (detectObject)
				{
					PlayerCharacter = Cast<ARPPlayerCharacter>(detectObject);

					if (PlayerCharacter)
					{
						RotateAndActivateAbility();
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
	else
	{
		return EBTNodeResult::Failed;
	}
}

EGDAbilityInputID URPBTTaskAttackPlayer::CanActivateAbility()
{
	EGDAbilityInputID abilityID = EGDAbilityInputID::None;

	float distanceBetweenActors = FVector::Distance(PlayerCharacter->GetActorLocation(), BotCharacter->GetActorLocation());

	if (distanceBetweenActors <= BotCharacter->GetLenghtToAttackFireBall())
	{
		abilityID = EGDAbilityInputID::FireBall;
	}

	if (distanceBetweenActors <= 100)
	{
		abilityID = EGDAbilityInputID::HandAttack;
	}

	return abilityID;
}

void URPBTTaskAttackPlayer::RotateAndActivateAbility()
{
	FRotator rotator = UKismetMathLibrary::FindLookAtRotation(BotCharacter->GetActorLocation(), PlayerCharacter->GetActorLocation());
	BotCharacter->SetActorRotation(rotator, ETeleportType::None);

	EGDAbilityInputID abilityToActivate = CanActivateAbility();

	if (abilityToActivate != EGDAbilityInputID::None)
	{
		BotCharacter->ActivateAbility(abilityToActivate);
	}
}

