// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"

#include "Kismet/KismetSystemLibrary.h"
#include "../../Characters/RPBaseCharacter.h"

#include "RPHandAttackAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class REVIEWPROJECT_API URPHandAttackAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime);

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);


private:
	bool bHitByCharacter = false;
};
