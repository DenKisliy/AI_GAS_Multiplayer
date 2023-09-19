// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EGDAbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	HandAttack,
	FireBall,
	Acceleration,
	Shield
};

UENUM(BlueprintType)
enum class EBuffAbilityID : uint8
{
	None,
	Acceleration,
	Shield
};