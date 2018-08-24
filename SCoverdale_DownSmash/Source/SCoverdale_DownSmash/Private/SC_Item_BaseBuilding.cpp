// Fill out your copyright notice in the Description page of Project Settings.

#include "SC_Item_BaseBuilding.h"

#include "SCoverdale_DownSmashCharacter.h"
#include "SC_BuildingBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"



ASC_Item_BaseBuilding::ASC_Item_BaseBuilding(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, MaxHeight(275.0f)
{

}

ASC_BuildingBase* ASC_Item_BaseBuilding::SpawnBuilding()
{
	ASC_BuildingBase* retVal = nullptr;

	if (BuildingClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			const FRotator SpawnRotation = OwningPawn->GetControlRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = OwningPawn->GetMuzzelLocation();

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			// spawn the projectile at the muzzle
			retVal = World->SpawnActor<ASC_BuildingBase>(BuildingClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}
	return retVal;
}

FHitResult ASC_Item_BaseBuilding::DoTrace(const FVector& Start, const FVector& End)
{
	FCollisionQueryParams TraceParams(TEXT("Trace"), true, Instigator);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;
	TraceParams.AddIgnoredActor(this);
	TraceParams.AddIgnoredActor(OwningPawn);
	TraceParams.AddIgnoredActor(CurrentBuilding);


	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_Vehicle, TraceParams);

	return Hit;
}

FVector ASC_Item_BaseBuilding::SnapLocationToGrid(FVector Location, int CellSize)
{
	FVector retVal = FVector::ZeroVector;

	retVal.X = FMath::RoundToFloat(Location.X / CellSize)* CellSize;
	retVal.Y = FMath::RoundToFloat(Location.Y / CellSize)* CellSize;
	retVal.Z = FMath::RoundToFloat(FMath::Min(Location.Z, MaxHeight) / CellSize)* CellSize;

	return retVal;
}

void ASC_Item_BaseBuilding::UseItem()
{
	//Spawn the building at the location and reset new Item
	if (CurrentBuilding && !CurrentBuilding->bHidden && !CurrentBuilding->HasOverlappingBuildings())
	{
		OwningPawn->AddGears(-GetGearCost());
		CurrentBuilding->OwningPawn = OwningPawn;
		CurrentBuilding->PlaceBuilding();
		ItemSelection();
		PlaySoundAndAnimation();

	}
}

void ASC_Item_BaseBuilding::UpdateItem()
{
	//Change Position based on raycast  and Change color of object if its legal in area

	if (CurrentBuilding)
	{
		const FVector& startingPos = OwningPawn->GetMuzzelLocation();
		const FVector& endpos = startingPos + (OwningPawn->GetControlRotation().Vector() * OwningPawn->GetBuildingDistance());
		const auto& hit = DoTrace(startingPos, endpos);

		/*DrawDebugLine(
			GetWorld(),
			startingPos,
			endpos,
			FColor(0, 255, 0),
			false, .1, 0,
			12.333
		);*/
		if (!hit.ImpactPoint.Equals(FVector::ZeroVector))
		{
			if (CurrentBuilding->bHidden)
				CurrentBuilding->SetActorHiddenInGame(false);

			CurrentBuilding->SetActorLocation(SnapLocationToGrid(hit.ImpactPoint, OwningPawn->GetCellSize()));
			CurrentBuilding->SetActorRotation(hit.ImpactNormal.Rotation() + FRotator(-90, 0, 0));
		}
		else
		{
			if(!CurrentBuilding->bHidden)
				CurrentBuilding->SetActorHiddenInGame(true);
		}

		if (CurrentBuilding->HasOverlappingBuildings())
		{

		}
	
	}

}

void ASC_Item_BaseBuilding::ItemSelection()
{
	//Spawn Default Object of actor
	CurrentBuilding = SpawnBuilding();
	
}

void ASC_Item_BaseBuilding::ItemDeselection()
{
	//Destroy DefaultObject
	if (CurrentBuilding)
		CurrentBuilding->Destroy();
}
