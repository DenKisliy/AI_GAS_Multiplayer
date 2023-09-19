// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "../UI/RPPlayerHUD.h"
#include "../Characters/RPPlayerCharacter.h"
#include "Blueprint/UserWidget.h"

#include "RPMainPlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class REVIEWPROJECT_API ARPMainPlayerHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<URPPlayerHUD> PlayerHUDWidget;

private:
	URPPlayerHUD* PlayerHUD;

protected:
	virtual void BeginPlay() override;
public:
	void CreatePlayerWidget();

	UFUNCTION(BlueprintCallable)
	void ShowPlayerHUD(bool Value);

	UFUNCTION(BlueprintCallable)
	void SetTimeForTimerWidget(int Time);

	void SetGameResultWin(bool bResult);

private:
	void SetDateForAttibutesWidget();

};
