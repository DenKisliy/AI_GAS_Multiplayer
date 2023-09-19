// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "Engine/DataTable.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "../Characters/RPAICharacter.h"
#include "../Characters/RPPlayerCharacter.h"
#include "Math/Vector.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "RPBotController.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FAIPointsForPatrolStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AI_Index = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level_Index = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Point = FVector(0, 0, 0);
};

class ARPBotCharacter;
class ARPPlayerCharacter;
class UAISenseConfig_Sight;
class UBlackboardData;
class UDataTable;

UCLASS()
class REVIEWPROJECT_API ARPBotController : public AAIController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere)
	UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere)
	UBlackboardComponent* BlackboardComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float PerceptionRadius = 500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float LosePerceptionRadius = 600;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BTBot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBlackboardData* BlackboardBot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UDataTable* DataTableWithAIPonts;

public:
	ARPBotController(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	void OnPerception(AActor* Actor, FAIStimulus Stimulus);

	TArray<FVector> GetPatrolPoints();

	void StartPatroling();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override; 
	
	virtual void OnPossess(APawn* InPawn) override;

public:
	UPROPERTY(BlueprintReadWrite)
	ARPAICharacter* Agent;

	UAISenseConfig_Sight* Sight;

	UAISenseConfig_Hearing* Hearing;

private:
	void SetDataInBlackboardBot();

	void GetDataFromDataTableWithAIPonts();
private:
	TArray<FVector> PatrolPoints;

	ARPPlayerCharacter* DetectPlayer;
};
