// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SC_AIDirector.h"
#include "SCoverdale_DownSmashGameMode.generated.h"


UCLASS(minimalapi)
class ASCoverdale_DownSmashGameMode : public AGameModeBase
{
	GENERATED_BODY()

	//Delay for the beginning of the game
	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	float GameStartDelay;

	//Delay between turns
	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	float TimeOutBetweenTurns;

	UPROPERTY(EditAnywhere, Category = "AIDirector")
	int TeamLives;
	
	bool bHasLostGame;


	int GetTeamLives();
	void AddTeamLives(int Amount);

	bool IsGameLost();

	FTimerHandle TimerHandle_HandleStartWave;

	ASC_AIDirector* AIDirector;

	void StartWave();

	UFUNCTION()
	void OnTurnFinished();

	UFUNCTION()
	void OnLifeLost();

	void OnGameLost();
	void OnGameRestart();



	void SetTimerForNextWave(float Time);

public:
	ASCoverdale_DownSmashGameMode();

	ASC_AIDirector* GetDirector();

	virtual void StartPlay() override;
};



