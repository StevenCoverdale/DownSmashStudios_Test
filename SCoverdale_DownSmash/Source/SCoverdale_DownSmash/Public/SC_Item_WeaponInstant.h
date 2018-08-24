// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SC_Item.h"
#include "SC_Item_WeaponInstant.generated.h"

/**
 * 
 */
UCLASS()
class SCOVERDALE_DOWNSMASH_API ASC_Item_WeaponInstant : public ASC_Item
{
	GENERATED_BODY()

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ASCoverdale_DownSmashProjectile> ProjectileClass;
	
	ASC_Item_WeaponInstant(const FObjectInitializer& ObjectInitializer);
	virtual void UseItem() override;
	virtual void UpdateItem() override;
	virtual void ItemSelection() override;
	virtual void ItemDeselection() override;


	
};
