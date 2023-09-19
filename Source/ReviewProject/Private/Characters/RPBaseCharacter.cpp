// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RPBaseCharacter.h"

// Sets default values
ARPBaseCharacter::ARPBaseCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);

	Attributes = CreateDefaultSubobject<URPAttributeSet>(TEXT("Attributes"));

	SpawnFireBallArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnFireBallArrow"));
	if (SpawnFireBallArrowComponent)
	{
		SpawnFireBallArrowComponent->ArrowColor = FColor(255, 0, 0);
		SpawnFireBallArrowComponent->bTreatAsASprite = true;
		SpawnFireBallArrowComponent->bIsScreenSizeScaled = true;
		SpawnFireBallArrowComponent->SetupAttachment(RootComponent);
	}

	SetReplicates(true);
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void ARPBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ARPBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		InitializeAttributes();
		GiveAbilities();
	}
}

void ARPBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	if (InputComponent && AbilitySystemComponent)
	{
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EGDAbilityInputID", static_cast<int32>(EGDAbilityInputID::Confirm), static_cast<int32>(EGDAbilityInputID::Cancel));
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}

void ARPBaseCharacter::SetRecoverAttributeEffect(TSubclassOf<UGameplayEffect> GameplayEffect)
{
	if (AbilitySystemComponent && GameplayEffect && !bInSafeArea)
	{
		FActiveGameplayEffectHandle GE = AbilitySystemComponent->ApplyGameplayEffectToSelf(GameplayEffect.GetDefaultObject(), 1, AbilitySystemComponent->MakeEffectContext());
	}
}

void ARPBaseCharacter::OnAutomaticRecoveryAttribute()
{
	if (GetHealthAttributeValue() == 0 || bInSafeArea)
	{
		GetWorld()->GetTimerManager().ClearTimer(AutomaticRecoveryAttributeTimerHandle);
	}
	else
	{
		if (GetHealthAttributeValue() < Attributes->GetMaxHealth() || GetManaAttributeValue() < Attributes->GetMaxMana() || GetStaminaAttributeValue() < Attributes->GetMaxStamina())
		{
			SetRecoverAttributeEffect(AutomaticRecoveryAttributeEffect);

			if (GetHealthAttributeValue() == Attributes->GetMaxHealth() && GetManaAttributeValue() == Attributes->GetMaxMana() && GetStaminaAttributeValue() == Attributes->GetMaxStamina())
			{
				GetWorld()->GetTimerManager().ClearTimer(AutomaticRecoveryAttributeTimerHandle);
			}
		}
	}
}

void ARPBaseCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AbilitySystemComponent->RefreshAbilityActorInfo();
}

void ARPBaseCharacter::InitializeAttributes()
{
	if (AbilitySystemComponent && DefaultAttributeEffect)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1, EffectContext);
		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle GE = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void ARPBaseCharacter::GiveAbilities()
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		for (TSubclassOf<URPBaseAbility>& Ability : DefaultAbilities)
		{
			if (Ability)
			{
				AbilitySystemComponent->GiveAbility(
					FGameplayAbilitySpec(Ability, 1, static_cast<int32>(Ability.GetDefaultObject()->AbilityInputID), this));
			}
		}
	}
}

// Called every frame
void ARPBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetHealthAttributeValue() == 0 && GetMaxHealthAttributeValue() != 0)
	{
		OnActorDestroed.Broadcast(this);
		Destroy();
	}

	if ((GetHealthAttributeValue() < GetMaxHealthAttributeValue() || GetManaAttributeValue() < GetMaxManaAttributeValue() || GetStaminaAttributeValue() < GetMaxStaminaAttributeValue()) && !(GetWorld()->GetTimerManager().IsTimerActive(AutomaticRecoveryAttributeTimerHandle)))
	{
		OnAutomaticRecoveryAttribute();
		GetWorld()->GetTimerManager().SetTimer(AutomaticRecoveryAttributeTimerHandle, this, &ARPBaseCharacter::OnAutomaticRecoveryAttribute, TimeOfAutomaticRecoveryAttribute, true);
	}
}

void ARPBaseCharacter::PlayDamageGameplayEffect(EGDAbilityInputID AbilityID, ARPBaseCharacter* target)
{
	if (!target->bActivShield)
	{
		TSubclassOf<UGameplayEffect> damageGameplayEffect;

		for (TSubclassOf<URPBaseAbility>& Ability : DefaultAbilities)
		{
			if (Ability)
			{
				if (Ability.GetDefaultObject()->AbilityInputID == AbilityID)
				{
					damageGameplayEffect = Ability.GetDefaultObject()->GetDamageGameplayEffect();
					if (damageGameplayEffect.GetDefaultObject())
					{
						AbilitySystemComponent->ApplyGameplayEffectToTarget(damageGameplayEffect.GetDefaultObject(), target->GetAbilitySystemComponent(), 1.f);
					}
				}
			}
		}
	}
}

void ARPBaseCharacter::PlayGameplayEffect(TSubclassOf<UGameplayEffect> GameplayEffect)
{
	if (GameplayEffect.GetDefaultObject())
	{
		AbilitySystemComponent->ApplyGameplayEffectToSelf(GameplayEffect.GetDefaultObject(), 1.f, GetAbilitySystemComponent()->MakeEffectContext());
	}
}

FVector ARPBaseCharacter::GetSpawnFireBallArrowLocation()
{
	return SpawnFireBallArrowComponent->GetComponentLocation();
}

void ARPBaseCharacter::SetManaAttributeValue(float Value)
{
	if (Attributes)
	{
		Attributes->SetMana(Value);
	}
}

void ARPBaseCharacter::SetStaminaAttributeValue(float Value)
{
	if (Attributes)
	{
		Attributes->SetStamina(Value);
	}
}

float ARPBaseCharacter::GetHealthAttributeValue()
{
	return  Attributes ? Attributes->GetHealth() : 0;
}

float ARPBaseCharacter::GetManaAttributeValue()
{
	return Attributes ? Attributes->GetMana() : 0;
}

float ARPBaseCharacter::GetStaminaAttributeValue()
{
	return Attributes ? Attributes->GetStamina() : 0;
}

float ARPBaseCharacter::GetMaxHealthAttributeValue()
{
	return Attributes ? Attributes->GetMaxHealth() : 0;
}

float ARPBaseCharacter::GetMaxManaAttributeValue()
{
	return Attributes ? Attributes->GetMaxMana() : 0;
}

float ARPBaseCharacter::GetMaxStaminaAttributeValue()
{
	return Attributes ? Attributes->GetMaxStamina() : 0;
}

void ARPBaseCharacter::MulticastPlayMontage_Implementation(UAnimMontage* PunchMontage, FName SectionName)
{
	PlayAnimMontage(PunchMontage, 1.f, SectionName);
}

