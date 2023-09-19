// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxComponent/RPSaveZone.h"

ARPSaveZone::ARPSaveZone()
{
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ARPSaveZone::OnUpdatedComponentOverlapBegin);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ARPSaveZone::OnUpdatedComponentOverlapEnd);
}

void ARPSaveZone::BeginPlay()
{
	Super::BeginPlay();
	CharacterInSaveZone();
}

void ARPSaveZone::OnUpdatedComponentOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UpdateCharacterSaveZoneValue(Other, true);
}

void ARPSaveZone::OnUpdatedComponentOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UpdateCharacterSaveZoneValue(Other, false);
}

void ARPSaveZone::UpdateCharacterSaveZoneValue(AActor* Other, bool Value)
{
	ARPPlayerCharacter* character = Cast<ARPPlayerCharacter>(Other);

	if (character)
	{
		character->CharacterFellIntoSafeArea(Value);

		if (Value)
		{
			character->PlayGameplayEffect(RecoverAttributeEffect);
		}
	}
}

void ARPSaveZone::CharacterInSaveZone()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, FoundActors);

	for (int i = 0; i < FoundActors.Num(); i++)
	{
		ARPPlayerCharacter* foundCharacter = Cast<ARPPlayerCharacter>(FoundActors[i]);
		if (foundCharacter)
		{
			if (!foundCharacter->IsCharacterInSafeArea())
			{
				foundCharacter->CharacterFellIntoSafeArea(IsPointInCube(foundCharacter->GetActorLocation()));
			}
		}
	}
}

bool ARPSaveZone::IsPointInCube(FVector Point)
{
	FVector saveAreaLocation = GetActorLocation();
	FVector scaledBoxExtent = BoxComponent->GetScaledBoxExtent();

	return (Point.X <= saveAreaLocation.X + scaledBoxExtent.X && Point.X >= saveAreaLocation.X - scaledBoxExtent.X
		&& Point.Y <= saveAreaLocation.Y + scaledBoxExtent.Y && Point.Y >= saveAreaLocation.Y - scaledBoxExtent.Y
		&& Point.Z <= saveAreaLocation.Z + scaledBoxExtent.Z && Point.Z >= saveAreaLocation.Z - scaledBoxExtent.Z);
}
