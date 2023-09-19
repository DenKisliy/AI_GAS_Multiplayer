// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "../../ReviewProject.h"
#include "Components/CapsuleComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "../GAS/Attribute/RPAttributeSet.h"
#include "../GAS/Ability/RPBaseAbility.h"
#include "Components/ArrowComponent.h"
#include "GameplayEffectTypes.h"
#include "Blueprint/WidgetTree.h"

#include "RPBaseCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActorDestroed, ARPBaseCharacter*, Player);

UCLASS()
class REVIEWPROJECT_API ARPBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARPBaseCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	URPAttributeSet* Attributes;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = GAS)
	TSubclassOf<UGameplayEffect> DefaultAttributeEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = GAS)
	TSubclassOf<UGameplayEffect> AutomaticRecoveryAttributeEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GAS)
	TArray<TSubclassOf<URPBaseAbility>> DefaultAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GAS)
	int32 TimeOfAutomaticRecoveryAttribute;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UArrowComponent* SpawnFireBallArrowComponent;
	
	UPROPERTY(BlueprintAssignable, Category = "Destroed callbacks")
	FActorDestroed OnActorDestroed;

protected:
	FTimerHandle AutomaticRecoveryAttributeTimerHandle;

	bool bInSafeArea = false;

	bool bActivShield = false;

	bool bActivateFireBall = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetRecoverAttributeEffect(TSubclassOf<UGameplayEffect> GameplayEffect);

	UFUNCTION()
	void OnAutomaticRecoveryAttribute();

	virtual void OnRep_PlayerState() override;

	void InitializeAttributes();

	void GiveAbilities();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PlayDamageGameplayEffect(EGDAbilityInputID AbilityID, ARPBaseCharacter* target);

	virtual void PlayGameplayEffect(TSubclassOf<UGameplayEffect> GameplayEffect);

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }

	FVector GetSpawnFireBallArrowLocation();

	void SetManaAttributeValue(float Value);

	void SetStaminaAttributeValue(float Value);

	UFUNCTION(BlueprintCallable)
	float GetHealthAttributeValue();

	UFUNCTION(BlueprintCallable)
	float GetManaAttributeValue();

	UFUNCTION(BlueprintCallable)
	float GetStaminaAttributeValue();

	UFUNCTION(BlueprintCallable)
	float GetMaxHealthAttributeValue();

	UFUNCTION(BlueprintCallable)
	float GetMaxManaAttributeValue();

	UFUNCTION(BlueprintCallable)
	float GetMaxStaminaAttributeValue();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayMontage(UAnimMontage* PunchMontage, FName SectionName);

};
