// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RPServerJoinButtonWidget.h"

void URPServerJoinButtonWidget::CreateElements(FOnlineSessionSearchResult SessionData)
{
	SessionInfo = SessionData;

	FString NameOfButton = "Empty Name";
	SessionInfo.Session.SessionSettings.Get(FName("SESSION_NAME_KEY"), NameOfButton);
	FString TextOfButton = NameOfButton;
	NameOfButton.RemoveSpacesInline();

	UButton* button = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), FName(*NameOfButton));

	UTextBlock* TextBlockOfButton = NewObject<UTextBlock>(this, UTextBlock::StaticClass());
	SessionInfo.Session.SessionSettings.Get(FName("SESSION_NAME_KEY"), NameOfButton);
	TextBlockOfButton->SetText(FText::FromString(TextOfButton));

	UScaleBox* buttonTextScaleBox = NewObject<UScaleBox>(this, UScaleBox::StaticClass());
	buttonTextScaleBox->SetStretch(EStretch::ScaleToFill);

	USizeBox* buttonTextSizeBox = NewObject<USizeBox>(this, USizeBox::StaticClass());
	buttonTextSizeBox->SetMinDesiredWidth(200.0f);
	buttonTextSizeBox->SetMinDesiredHeight(40.0f);
	buttonTextSizeBox->AddChild(TextBlockOfButton);
	buttonTextScaleBox->AddChild(buttonTextSizeBox);

	button->AddChild(buttonTextScaleBox);

	button->OnClicked.AddDynamic(this, &URPServerJoinButtonWidget::OnJoinToSession);

	HorizontalBoxElement->AddChild(button);
}

void URPServerJoinButtonWidget::OnJoinToSession()
{
	URPSessionSubsystem* SessionManager = GetWorld()->GetSubsystem<URPSessionSubsystem>();

	if (SessionManager)
	{
		SessionManager->JoinServer(SessionInfo);
	}
}