// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SCoverdale_DownSmashGameMode.h"
#include "SCoverdale_DownSmashHUD.h"
#include "SCoverdale_DownSmashCharacter.h"
#include "SC_PointOfInterest.h"
#include "UObject/ConstructorHelpers.h"

ASCoverdale_DownSmashGameMode::ASCoverdale_DownSmashGameMode()
	: Super()
	, GameStartDelay(30)
	, TimeOutBetweenTurns(15)
	, TeamLives(10)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ASCoverdale_DownSmashHUD::StaticClass();
}

int ASCoverdale_DownSmashGameMode::GetTeamLives()
{
	return TeamLives;
}

void ASCoverdale_DownSmashGameMode::AddTeamLives(int Amount)
{
	TeamLives += Amount;
	if (TeamLives <= 0)
	{
		bHasLostGame = true;
		OnGameLost();
	}
}

bool ASCoverdale_DownSmashGameMode::IsGameLost()
{
	return bHasLostGame;
}

void ASCoverdale_DownSmashGameMode::StartWave()
{
	GetDirector()->SpawnEnemies();
}

void ASCoverdale_DownSmashGameMode::OnTurnFinished()
{
	if(!IsGameLost())
		SetTimerForNextWave(TimeOutBetweenTurns);
}

void ASCoverdale_DownSmashGameMode::OnLifeLost()
{
	AddTeamLives(-1);
	AIDirector->UnitKilled();
}

void ASCoverdale_DownSmashGameMode::OnGameLost()
{

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, FString::Printf(TEXT("GAME OVER: ROUND LOST - PREPAIR FOR RESTART")));


	GetWorldTimerManager().SetTimer(TimerHandle_HandleStartWave, this, &ASCoverdale_DownSmashGameMode::OnGameRestart, 5, false);
}

void ASCoverdale_DownSmashGameMode::OnGameRestart()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("RestartLevel");

}

void ASCoverdale_DownSmashGameMode::SetTimerForNextWave(float Time)
{
	GetWorldTimerManager().SetTimer(TimerHandle_HandleStartWave, this, &ASCoverdale_DownSmashGameMode::StartWave, Time, false);

}

ASC_AIDirector* ASCoverdale_DownSmashGameMode::GetDirector()
{
	if (!AIDirector)
	{
		AIDirector = ASC_AIDirector::GetAIDirector(this);
		AIDirector->OnTurnReady.AddUniqueDynamic(this, &ASCoverdale_DownSmashGameMode::OnTurnFinished);
		if(AIDirector->EndPoint)
			AIDirector->EndPoint->LivesLost.AddUniqueDynamic(this, &ASCoverdale_DownSmashGameMode::OnLifeLost);
	}

	return AIDirector;
}

void ASCoverdale_DownSmashGameMode::StartPlay()
{
	Super::StartPlay();
	SetTimerForNextWave(GameStartDelay);
}
