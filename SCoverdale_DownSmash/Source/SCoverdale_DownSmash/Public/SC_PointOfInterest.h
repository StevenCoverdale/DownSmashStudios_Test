// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "SC_PointOfInterest.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPOIEvent);


UCLASS()
class SCOVERDALE_DOWNSMASH_API ASC_PointOfInterest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASC_PointOfInterest();

	FPOIEvent LivesLost;


	UPROPERTY(VisibleAnywhere, Category = "Building|Trap")
	UBoxComponent* BoxCollider;

	UPROPERTY(EditAnywhere, Category = "PointOfInterest")
	bool bIsStartingPoint;

	UPROPERTY(EditAnywhere, Category = "PointOfInterest")
	ASC_PointOfInterest* EndGoal;

	FVector GetEndPointLocation() const;

	bool IsStartingPoint() const;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
