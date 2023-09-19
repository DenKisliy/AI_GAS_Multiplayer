// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/BoxComponent.h"
#include "RPBaseBoxComponent.generated.h"

UCLASS()
class REVIEWPROJECT_API ARPBaseBoxComponent : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComponent;

public:	
	// Sets default values for this actor's properties
	ARPBaseBoxComponent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
