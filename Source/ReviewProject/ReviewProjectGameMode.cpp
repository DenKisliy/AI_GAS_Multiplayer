// Copyright Epic Games, Inc. All Rights Reserved.

#include "ReviewProjectGameMode.h"

#include "UObject/ConstructorHelpers.h"

AReviewProjectGameMode::AReviewProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ReviewProject/Characters/BP_PlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default static class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<AGameStateBase> GameStateBPClass(TEXT("/Game/GS_ReviewProject"));
	if (GameStateBPClass.Class != NULL)
	{
		GameStateClass = GameStateBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<AHUD> HUDBPClass(TEXT("/Game/ReviewProject/UI/WBP_MainHUD"));
	if (HUDBPClass.Class != NULL)
	{
		HUDClass = HUDBPClass.Class;
	}
}

