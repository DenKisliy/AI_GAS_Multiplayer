// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "RPServerJoinButtonWidget.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/EditableText.h"
#include "Components/CanvasPanel.h"
#include "Components/SizeBox.h"
#include "Components/ScaleBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/EditableTextBox.h"
#include "Components/CanvasPanel.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/GameplayStatics.h"
#include "../GameState/RPGameState.h"

#include "RPMainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class REVIEWPROJECT_API URPMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<URPServerJoinButtonWidget> ServerJoinButtonWidget;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* MainMenuCanvasPanel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UHorizontalBox* CommandsHorizontalBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UHorizontalBox* ServersHorizontalBox;

	class UEditableTextBox* SessionNameTextBox;

	virtual void OnFindSessionComplete(bool Succeeded);

public:
	UFUNCTION(BlueprintCallable, Category = "DebugMenu")
	void ShowMainMenuWidget();

	UFUNCTION()
	void CreateSession();

	UFUNCTION()
	void FindSessions();

private:
	UButton* CreateCommandButton(FString NameOfButton, FName FunctionName);

	void SetTextForRightPart(FString Text);
};
