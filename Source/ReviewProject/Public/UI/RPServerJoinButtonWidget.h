// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"
#include "Components/ScaleBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Kismet/GameplayStatics.h"
#include "../Subsystem/RPSessionSubsystem.h"
#include "Kismet/GameplayStatics.h"

#include "RPServerJoinButtonWidget.generated.h"

/**
 * 
 */
UCLASS()
class REVIEWPROJECT_API URPServerJoinButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* DebugCanvasPanel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* HorizontalBoxElement;

private:
	FOnlineSessionSearchResult SessionInfo;

public:
	void CreateElements(FOnlineSessionSearchResult SessionData);

	UFUNCTION()
	void OnJoinToSession();
};
