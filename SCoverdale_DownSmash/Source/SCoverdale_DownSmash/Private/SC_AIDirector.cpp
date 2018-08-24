// Fill out your copyright notice in the Description page of Project Settings.

#include "SC_AIDirector.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASC_AIDirector::ASC_AIDirector()
	: Super()
	, NumberOfEnemiesModifier(1.1f)
	, BaseNumberOfEnemies(10)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ASC_AIDirector::AdjustNumberOfEnemiesFromModifier()
{
	BaseNumberOfEnemies *=  NumberOfEnemiesModifier;
}

void ASC_AIDirector::SpawnEnemies()
{
	if(SpawnedEnemies < BaseNumberOfEnemies && ClassesOfEnemies.Num() > 0)
	{
		auto chosenEnemyClass = ClassesOfEnemies[FMath::RandRange(0, ClassesOfEnemies.Num() - 1)];
		
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			const FRotator SpawnRotation = StartingPoint->GetActorRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = StartingPoint->GetActorLocation();

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			// spawn the projectile at the muzzle
			auto AiBot = World->SpawnActor<ASC_AIDownSmashCharacter>(chosenEnemyClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

			if (AiBot)
			{
				AiBot->OnUnitKilled.AddUniqueDynamic(this, &ASC_AIDirector::UnitKilled);
				AiBot->AddHealth(AiBot->GetHealth() * NumberOfEnemiesModifier);
				
				SpawnedEnemies++;
			}
		}

		GetWorldTimerManager().SetTimer(TimerHandle_HandleSpawning, this, &ASC_AIDirector::SpawnEnemies, TimeBetweenUnitsSpawning, false);
	}
}

void ASC_AIDirector::UnitKilled()
{
	EnemiesKilledThisRound++;
	if (EnemiesKilledThisRound >= BaseNumberOfEnemies)
	{
		RoundCleared();
	}
}

void ASC_AIDirector::RoundCleared()
{
	OnTurnReady.Broadcast();
	AdjustNumberOfEnemiesFromModifier();

	EnemiesKilledThisRound = 0;
	SpawnedEnemies = 0;
}

// Called when the game starts or when spawned
void ASC_AIDirector::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASC_AIDirector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

ASC_AIDirector* ASC_AIDirector::GetAIDirector(const UObject* WorldContextObject)
{
	ASC_AIDirector* retVal = nullptr;
	TArray<AActor*> Found;
	UGameplayStatics::GetAllActorsOfClass(WorldContextObject, ASC_AIDirector::StaticClass(), Found);

	if (Found.Num() > 0)
	{
		ASC_AIDirector* dir = Cast<ASC_AIDirector>(Found[0]);
		retVal = dir;
	}

	return retVal;
}

