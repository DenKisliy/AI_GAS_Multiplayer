// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/RPBaseCharacter.h"
#include "RPAICharacter.generated.h"

/**
 * 
 */
UCLASS()
class REVIEWPROJECT_API ARPAICharacter : public ARPBaseCharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 AIIndex = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int LenghtToAttackFireBall = 500;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	int32 GetAIIndex() { return AIIndex; }

	void SetAIIndex(int32 Index);

	int GetLenghtToAttackFireBall() { return LenghtToAttackFireBall; }

	void ActivateAbility(EGDAbilityInputID AbilityID);
};
