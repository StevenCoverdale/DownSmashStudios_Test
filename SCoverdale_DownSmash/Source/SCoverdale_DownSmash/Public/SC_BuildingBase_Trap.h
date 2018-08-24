// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SC_BuildingBase.h"
#include "Components/BoxComponent.h"
#include "SC_BuildingBase_Trap.generated.h"

/**
 * 
 */
UCLASS()
class SCOVERDALE_DOWNSMASH_API ASC_BuildingBase_Trap : public ASC_BuildingBase
{
	GENERATED_BODY()


	ASC_BuildingBase_Trap();

	UPROPERTY(VisibleAnywhere, Category = "Building|Trap")
	UBoxComponent* BoxCollider;

	UPROPERTY(EditDefaultsOnly, Category = "Building|Trap")
	float MaxRange;

	virtual void PlaceBuilding() override;

protected:

	FHitResult DoTrace(const FVector& Start, const FVector& End);

	void SetCollisionLocation();
	


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void HandleAttack() override;

public:
	virtual TArray<AActor*> GetOverlappingPawns() override;
};
