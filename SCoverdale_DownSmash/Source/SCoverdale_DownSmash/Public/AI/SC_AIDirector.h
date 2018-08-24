// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SC_PointOfInterest.h"
#include "SC_AIDownSmashCharacter.h"
#include "SC_AIDirector.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDirectorEvent);

UCLASS()
class SCOVERDALE_DOWNSMASH_API ASC_AIDirector : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASC_AIDirector();

	FDirectorEvent OnTurnReady;

	UPROPERTY(EditAnywhere, Category = "AIDirector")
	ASC_PointOfInterest* StartingPoint;

	UPROPERTY(EditAnywhere, Category = "AIDirector")
	ASC_PointOfInterest* EndPoint;

	UPROPERTY(EditAnywhere, Category = "AIDirector")
	TArray<TSubclassOf<class ASC_AIDownSmashCharacter>> ClassesOfEnemies;
	
	int SpawnedEnemies;

	FTimerHandle TimerHandle_HandleSpawning;

	UPROPERTY(EditDefaultsOnly, Category = "AIDirector")
	float TimeBetweenUnitsSpawning;

	UPROPERTY(EditDefaultsOnly, Category = "AIDirector")
	float NumberOfEnemiesModifier;

	UPROPERTY(EditDefaultsOnly, Category = "AIDirector")
	int BaseNumberOfEnemies;

	int EnemiesKilledThisRound;

	void AdjustNumberOfEnemiesFromModifier();

	void SpawnEnemies();

	UFUNCTION()
	void UnitKilled();
	
	void RoundCleared();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "System", meta = (WorldContext = "WorldContextObject"))
	static ASC_AIDirector* GetAIDirector(const UObject* WorldContextObject);
	
};
