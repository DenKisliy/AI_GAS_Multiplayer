// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/RPSessionSubsystem.h"

void URPSessionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (IOnlineSubsystem* subSystem = IOnlineSubsystem::Get())
	{
		if (subSystem)
		{
			SessionInterface = subSystem->GetSessionInterface();
			if (SessionInterface.IsValid())
			{
				SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &URPSessionSubsystem::OnCreateSessionComplete);
				SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &URPSessionSubsystem::OnJoinSessionComplete);
			}
		}
	}
}

void URPSessionSubsystem::CreateServer(FName SessionName)
{
	FOnlineSessionSettings sessionSettings;
	sessionSettings.bAllowJoinInProgress = true;
	sessionSettings.bIsDedicated = false;

	sessionSettings.bIsLANMatch = true;
	sessionSettings.bShouldAdvertise = true;
	sessionSettings.bUsesPresence = true;

	sessionSettings.NumPublicConnections = 2;

	ARPGameState* const gameState = GetWorld() != NULL ? GetWorld()->GetGameState<ARPGameState>() : NULL;

	if (gameState)
	{
		sessionSettings.NumPublicConnections = gameState->PlayerCountOfConnection;
	}

	sessionSettings.Set(FName("SESSION_NAME_KEY"), SessionName.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	SessionInterface->CreateSession(0, SessionName, sessionSettings);
}

void URPSessionSubsystem::FindServers()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());

	SessionSearch->bIsLanQuery = true;
	SessionSearch->MaxSearchResults = 10000;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void URPSessionSubsystem::JoinServer(FOnlineSessionSearchResult Session)
{
	FString SessionName = "Empty Name";
	Session.Session.SessionSettings.Get(FName("SESSION_NAME_KEY"), SessionName);
	SessionInterface->JoinSession(0, *SessionName, Session);
}

void URPSessionSubsystem::OnCreateSessionComplete(FName ServerName, bool Succeeded)
{
	if (Succeeded)
	{
		FString JoinAddress = "";
		SessionInterface->GetResolvedConnectString(ServerName, JoinAddress);
		LoadLevelToPlay();
	}
}

void URPSessionSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		APlayerController* PController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PController)
		{
			FString JoinAddress = "";
			SessionInterface->GetResolvedConnectString(SessionName, JoinAddress);

			if (JoinAddress != "")
			{
				PController->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);
			}
		}
	}
}

void URPSessionSubsystem::LoadLevelToPlay()
{
	GetWorld()->ServerTravel(TEXT("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen"), false);
}