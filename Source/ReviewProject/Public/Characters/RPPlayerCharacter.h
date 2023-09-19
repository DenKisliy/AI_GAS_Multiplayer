// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/RPBaseCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../UI/RPPlayerHUD.h"
#include "../BuffActor/RPBuffActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"

#include "RPPlayerCharacter.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeChange, float, AttributeValue);

UCLASS()
class REVIEWPROJECT_API ARPPlayerCharacter : public ARPBaseCharacter
{
	GENERATED_BODY()

public:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere)
	TSubclassOf<URPPlayerHUD> PlayerHUDWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ARPBuffActor> BuffActor;

	UPROPERTY(BlueprintAssignable, Category = "Attribute callbacks")
	FAttributeChange OnHealthChange;

	UPROPERTY(BlueprintAssignable, Category = "Attribute callbacks")
	FAttributeChange OnManaChange;

	UPROPERTY(BlueprintAssignable, Category = "Attribute callbacks")
	FAttributeChange OnStaminaChange;

	bool bCanUseAbility = false;

private:
	float DefaultCharacterSpeed;

public:
	ARPPlayerCharacter();

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	bool IsCharacterInSafeArea() { return bInSafeArea; }

	void CharacterFellIntoSafeArea(bool value);

	UFUNCTION(NetMulticast, Reliable)
	void ActivateAcceleration(float Coefficient, int TimeToDeactivate);

	UFUNCTION(NetMulticast, Reliable)
	void ActivateVisualShield(int TimeToDeactivate);

	UFUNCTION(NetMulticast, Reliable)
	void DisableAbility(EBuffAbilityID BuffID);

	void PlayDamageGameplayEffect(EGDAbilityInputID AbilityID, ARPBaseCharacter* target) override;

	// The callback to be registered within AbilitySystem.
	void OnManaUpdated(const FOnAttributeChangeData& Data);

	// The callback to be registered within AbilitySystem.
	void OnHealthUpdated(const FOnAttributeChangeData& Data);

	// The callback to be registered within AbilitySystem.
	void OnStaminaUpdated(const FOnAttributeChangeData& Data);

protected:
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);
};
