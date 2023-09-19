// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "../../ReviewProject.h"

#include "RPBuffActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDisableBuff, EBuffAbilityID, BuffID);

UCLASS()
class REVIEWPROJECT_API ARPBuffActor : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* ParticleComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Shield Abilities")
	UParticleSystem* ShieldEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Shield Abilities")
	FName ShieldSocket;

	UPROPERTY(EditDefaultsOnly, Category = "Acceleration Abilities")
	UParticleSystem* AccelerationEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Acceleration Abilities")
	FName AccelerationSocket;
	
	UPROPERTY(BlueprintAssignable, Category = "Attribute callbacks")
	FDisableBuff OnDisableBuff;

private:
	FTimerHandle DestroyTimerHandle;
	
	EBuffAbilityID ActivBuff = EBuffAbilityID::None;
public:	
	// Sets default values for this actor's properties
	ARPBuffActor();

	void ActivateBuff(EBuffAbilityID BuffID, int TimeToDeactivate);

private:
	UFUNCTION()
	void OnDestroy();
};
