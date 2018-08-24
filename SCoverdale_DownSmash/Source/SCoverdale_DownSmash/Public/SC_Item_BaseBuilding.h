// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SC_Item.h"
#include "SC_Item_BaseBuilding.generated.h"

/**
 * 
 */
UCLASS()
class SCOVERDALE_DOWNSMASH_API ASC_Item_BaseBuilding : public ASC_Item
{
	GENERATED_BODY()

	ASC_Item_BaseBuilding(const FObjectInitializer& ObjectInitializer);

	/** Building class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Building)
	TSubclassOf<class ASC_BuildingBase> BuildingClass;

	UPROPERTY(EditDefaultsOnly, Category = "Building")
	float MaxHeight;

	ASC_BuildingBase* CurrentBuilding;

	ASC_BuildingBase* SpawnBuilding();

	FHitResult DoTrace(const FVector& Start, const FVector& End);

	FVector SnapLocationToGrid(FVector Location, int CellSize);

	virtual void UseItem() override;
	virtual void UpdateItem() override;
	virtual void ItemSelection() override;
	virtual void ItemDeselection() override;
	
	
};
