// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RPPlayerCharacter.h"


ARPPlayerCharacter::ARPPlayerCharacter()
{
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm}
	
	DefaultCharacterSpeed = GetCharacterMovement()->GetMaxSpeed();

	SetReplicates(true);
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void ARPPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ARPPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void ARPPlayerCharacter::CharacterFellIntoSafeArea(bool value)
{
	bInSafeArea = value;
}

void ARPPlayerCharacter::ActivateAcceleration_Implementation(float Coefficient, int TimeToDeactivate)
{
	float speed = GetCharacterMovement()->GetMaxSpeed();
	FActorSpawnParameters SpawnParams;

	ARPBuffActor* acceleration = GetWorld()->SpawnActor<ARPBuffActor>(BuffActor, GetActorLocation(), GetActorRotation(), SpawnParams);
	acceleration->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, acceleration->AccelerationSocket);
	acceleration->ActivateBuff(EBuffAbilityID::Acceleration, TimeToDeactivate);
	acceleration->OnDisableBuff.AddDynamic(this, &ARPPlayerCharacter::DisableAbility);

	GetCharacterMovement()->MaxWalkSpeed = speed * Coefficient;
}

void ARPPlayerCharacter::ActivateVisualShield_Implementation(int TimeToDeactivate)
{
	FActorSpawnParameters SpawnParams;
	ARPBuffActor* shield = GetWorld()->SpawnActor<ARPBuffActor>(BuffActor, GetActorLocation(), GetActorRotation(), SpawnParams);
	shield->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, shield->ShieldSocket);
	shield->ActivateBuff(EBuffAbilityID::Shield, TimeToDeactivate);

	FRotator socketRotation = GetMesh()->GetSocketRotation(shield->ShieldSocket);
	socketRotation.Pitch = 0.0f;
	socketRotation.Yaw = 0.0f;
	socketRotation.Roll = 0.0f;
	shield->SetActorRotation(socketRotation);

	shield->OnDisableBuff.AddDynamic(this, &ARPPlayerCharacter::DisableAbility);

	bActivShield = true;
}

void ARPPlayerCharacter::DisableAbility_Implementation(EBuffAbilityID BuffID)
{
	switch (BuffID)
	{
	case EBuffAbilityID::Acceleration:
		GetCharacterMovement()->MaxWalkSpeed = DefaultCharacterSpeed;
		break;
	case EBuffAbilityID::Shield:
		bActivShield = false;
		break;
	}
}

void ARPPlayerCharacter::PlayDamageGameplayEffect(EGDAbilityInputID AbilityID, ARPBaseCharacter* target)
{
	if (!bInSafeArea || !bActivShield)
	{
		Super::PlayDamageGameplayEffect(AbilityID, target);
	}
}

void ARPPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ARPPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARPPlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ARPPlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ARPPlayerCharacter::LookUpAtRate);

	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ARPPlayerCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ARPPlayerCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ARPPlayerCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ARPPlayerCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ARPPlayerCharacter::OnManaUpdated(const FOnAttributeChangeData& Data)
{
	OnManaChange.Broadcast(Data.NewValue);
}

void ARPPlayerCharacter::OnHealthUpdated(const FOnAttributeChangeData& Data)
{
	OnHealthChange.Broadcast(Data.NewValue);
}

void ARPPlayerCharacter::OnStaminaUpdated(const FOnAttributeChangeData& Data)
{
	OnStaminaChange.Broadcast(Data.NewValue);
}
