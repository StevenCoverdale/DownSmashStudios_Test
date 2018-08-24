// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SC_PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SCOVERDALE_DOWNSMASH_API ASC_PlayerState : public APlayerState
{
	GENERATED_BODY()

	float Health;
	float Gears;
	float Mana;
	
};
