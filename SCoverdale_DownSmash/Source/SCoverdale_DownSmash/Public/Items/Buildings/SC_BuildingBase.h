// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "SC_BuildingBase.generated.h"

UCLASS()
class SCOVERDALE_DOWNSMASH_API ASC_BuildingBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASC_BuildingBase();

	UPROPERTY(VisibleAnywhere, Category = "Building")
	UStaticMeshComponent* BaseMesh;

	USceneComponent* Root;

	FTimerHandle TimerHandle_HandleFiring;

	AActor* OwningPawn;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float ShotsPerMinute;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float Damage;

	AActor* CurrentTarget;

	float LastFireTime;

	/* Time between shots for repeating fire */
	float TimeBetweenShots;

	UFUNCTION()
	void OnOverlapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintImplementableEvent, Category = "BuildingBase")
	bool ToggleBlueprintCollision(bool Enable);
	virtual bool ToggleBlueprintCollision_Implementable(bool Enable) { return false; };

	bool HasOverlappingBuildings();

	bool CanPawnBeAttacked(AActor* Target);

	virtual TArray<AActor*> GetOverlappingPawns();

	virtual void PlaceBuilding();


	void OnAttackStarted();

	AActor* GetTarget();
	void SetTarget(AActor* Target);

	bool ApplyDamage(float InDamage);

protected:
	
	virtual void HandleAttack();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
