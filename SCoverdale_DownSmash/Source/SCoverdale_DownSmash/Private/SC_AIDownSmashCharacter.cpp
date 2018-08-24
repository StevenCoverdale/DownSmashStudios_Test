// Fill out your copyright notice in the Description page of Project Settings.

#include "SC_AIDownSmashCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "SCoverdale_DownSmashProjectile.h"
#include "SC_PointOfInterest.h"


ASC_AIDownSmashCharacter::ASC_AIDownSmashCharacter(const FObjectInitializer& ObjectInitializer)
{

}

FVector ASC_AIDownSmashCharacter::GetFinalDestination()
{
	FVector retval = FVector::ZeroVector;
	TArray<AActor*> Found;
	UGameplayStatics::GetAllActorsOfClass(this, ASC_PointOfInterest::StaticClass(), Found);

	for (auto poi : Found)
	{
		ASC_PointOfInterest* pPoi = Cast<ASC_PointOfInterest>(poi);
		if (pPoi->IsStartingPoint())
		{
			retval = pPoi->GetEndPointLocation();
			break;
		}
	}

	return retval;
}

bool ASC_AIDownSmashCharacter::CanDealDamage_Implementation(UObject* FromObj /*= nullptr*/)
{
	//ASCoverdale_DownSmashProjectile* proj = Cast<ASCoverdale_DownSmashProjectile>(FromObj);

	if (IsAlive() /*&& proj*/)
		return true;
	else
		Destroy();

	return false;
}

void ASC_AIDownSmashCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASC_AIDownSmashCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ASC_AIDownSmashCharacter::SetupPlayerInputComponent(UInputComponent* InputComponent)
{

}
