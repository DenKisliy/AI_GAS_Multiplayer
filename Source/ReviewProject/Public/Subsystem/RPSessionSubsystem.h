// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "GameFramework/PlayerController.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Kismet/GameplayStatics.h"
#include "../GameState/RPGameState.h"

#include "RPSessionSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class REVIEWPROJECT_API URPSessionSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	IOnlineSessionPtr SessionInterface;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void OnCreateSessionComplete(FName ServerName, bool Succeeded);

	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

private:
	UFUNCTION()
	void LoadLevelToPlay();

public:
	void JoinServer(FOnlineSessionSearchResult Session);

	UFUNCTION()
	void CreateServer(FName SessionName);

	UFUNCTION()
	void FindServers();
};
