// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IHasDamage.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHasDamage : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SCOVERDALE_DOWNSMASH_API IHasDamage
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage")
	void GiveKillReward(int Reward);
	virtual void GiveKillReward_Implementation(int Reward){}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage")
	bool CanDealDamage(UObject* FromObj = nullptr);
	virtual bool CanDealDamage_Implementation(UObject* FromObj = nullptr) { return false; }

	
};
