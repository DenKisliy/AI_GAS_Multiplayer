// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RPMainMenuWidget.h"

void URPMainMenuWidget::ShowMainMenuWidget()
{
	UVerticalBox* testVerticalBox = NewObject<UVerticalBox>(this, UVerticalBox::StaticClass());
	UVerticalBox* commandVerticalBox = NewObject<UVerticalBox>(this, UVerticalBox::StaticClass());
	UHorizontalBox* debugMenuHorizontalBox = NewObject<UHorizontalBox>(this, UHorizontalBox::StaticClass());

	UScrollBox* DebugMenuScrollBox = NewObject<UScrollBox>(this, UScrollBox::StaticClass());
	UTextBlock* textHeaderCommandsTextBlock = NewObject<UTextBlock>(this, UTextBlock::StaticClass());

	commandVerticalBox->AddChild(DebugMenuScrollBox);

	debugMenuHorizontalBox->AddChild(commandVerticalBox);
	textHeaderCommandsTextBlock->SetText(FText::FromString("Commands"));

	DebugMenuScrollBox->AddChild(textHeaderCommandsTextBlock);

	FString nameOfButton;
	FName functionOfButton;
	UButton* cretedButton;

	nameOfButton = "Create Session";
	functionOfButton = "CreateSession";

	cretedButton = CreateCommandButton(nameOfButton, functionOfButton);

	DebugMenuScrollBox->AddChild(cretedButton);

	UHorizontalBox* CreateSessionNameHorizontalBox = NewObject<UHorizontalBox>(this, UHorizontalBox::StaticClass());

	UTextBlock* textSessionNameTextBlock = NewObject<UTextBlock>(this, UTextBlock::StaticClass());
	textSessionNameTextBlock->SetText(FText::FromString("Session name: "));

	CreateSessionNameHorizontalBox->AddChild(textSessionNameTextBlock);

	SessionNameTextBox = NewObject<UEditableTextBox>(this, UEditableTextBox::StaticClass());
	SessionNameTextBox->SetToolTipText(FText::FromString("Server Name"));
	UButton* button = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), FName("ServerNameTaxt"));

	UScaleBox* buttonTextScaleBox = NewObject<UScaleBox>(this, UScaleBox::StaticClass());
	buttonTextScaleBox->SetStretch(EStretch::ScaleToFill);

	USizeBox* buttonTextSizeBox = NewObject<USizeBox>(this, USizeBox::StaticClass());
	buttonTextSizeBox->SetMinDesiredWidth(200.0f);
	buttonTextSizeBox->SetMinDesiredHeight(20.0f);
	buttonTextSizeBox->AddChild(SessionNameTextBox);
	buttonTextScaleBox->AddChild(buttonTextSizeBox);

	button->AddChild(buttonTextScaleBox);

	CreateSessionNameHorizontalBox->AddChild(button);

	DebugMenuScrollBox->AddChild(CreateSessionNameHorizontalBox);

	nameOfButton = "Find Sessions";
	functionOfButton = "FindSessions";

	cretedButton = CreateCommandButton(nameOfButton, functionOfButton);

	DebugMenuScrollBox->AddChild(cretedButton);

	CommandsHorizontalBox->AddChild(commandVerticalBox);

	URPSessionSubsystem* SessionManager = GetWorld()->GetSubsystem<URPSessionSubsystem>();

	if (SessionManager)
	{
		SessionManager->SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &URPMainMenuWidget::OnFindSessionComplete);
	}
}

void URPMainMenuWidget::CreateSession()
{
	FString name = (SessionNameTextBox->GetText()).ToString();
	URPSessionSubsystem* SessionManager = GetWorld()->GetSubsystem<URPSessionSubsystem>();

	if (SessionManager)
	{
		SessionManager->CreateServer(FName(*name));
	}
}

void URPMainMenuWidget::FindSessions()
{
	UTextBlock* textHeaderCommandsTextBlock = NewObject<UTextBlock>(this, UTextBlock::StaticClass());
	textHeaderCommandsTextBlock->SetText(FText::FromString("Search Sessions"));
	ServersHorizontalBox->AddChild(textHeaderCommandsTextBlock);

	URPSessionSubsystem* SessionManager = GetWorld()->GetSubsystem<URPSessionSubsystem>();

	if (SessionManager)
	{
		SessionManager->FindServers();
	}
}

UButton* URPMainMenuWidget::CreateCommandButton(FString NameOfButton, FName FunctionName)
{
	UButton* button = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), FName(*NameOfButton));
	FScriptDelegate Delegate;
	Delegate.BindUFunction(this, FunctionName);

	button->OnClicked.Add(Delegate);

	UTextBlock* TextBlockOfButton = NewObject<UTextBlock>(this, UTextBlock::StaticClass());
	TextBlockOfButton->SetText(FText::FromString(NameOfButton));

	UScaleBox* buttonTextScaleBox = NewObject<UScaleBox>(this, UScaleBox::StaticClass());
	buttonTextScaleBox->SetStretch(EStretch::ScaleToFill);

	USizeBox* buttonTextSizeBox = NewObject<USizeBox>(this, USizeBox::StaticClass());
	buttonTextSizeBox->SetMinDesiredWidth(200.0f);
	buttonTextSizeBox->AddChild(TextBlockOfButton);
	buttonTextScaleBox->AddChild(buttonTextSizeBox);

	button->AddChild(buttonTextScaleBox);

	return button;
}

void URPMainMenuWidget::SetTextForRightPart(FString Text)
{
	ServersHorizontalBox->ClearChildren();
	UTextBlock* textHeaderCommandsTextBlock = NewObject<UTextBlock>(this, UTextBlock::StaticClass());
	textHeaderCommandsTextBlock->SetText(FText::FromString(Text));
	ServersHorizontalBox->AddChild(textHeaderCommandsTextBlock);
}


void URPMainMenuWidget::OnFindSessionComplete(bool Succeeded)
{
	URPSessionSubsystem* SessionManager = GetWorld()->GetSubsystem<URPSessionSubsystem>();

	if (SessionManager)
	{
		
		if (Succeeded)
		{
			if (SessionManager->SessionSearch)
			{
				TArray<FOnlineSessionSearchResult> SearchResults = SessionManager->SessionSearch->SearchResults;

				if (SearchResults.Num() > 0)
				{
					UVerticalBox* commandVerticalBox = NewObject<UVerticalBox>(this, UVerticalBox::StaticClass());
					ServersHorizontalBox->ClearChildren();
					for (FOnlineSessionSearchResult Result : SearchResults)
					{
						if (!Result.IsValid())
							continue;

						FString ServerName = "Empty Name";
						Result.Session.SessionSettings.Get(FName("SESSION_NAME_KEY"), ServerName);

						URPServerJoinButtonWidget* button = CreateWidget<URPServerJoinButtonWidget>(this, ServerJoinButtonWidget);
						button->CreateElements(Result);
						commandVerticalBox->AddChild(button);
					}
					ServersHorizontalBox->AddChild(commandVerticalBox);
				}
				else
				{
					SetTextForRightPart("Unable to find sessions");
				}
			}
		}
	}
}
