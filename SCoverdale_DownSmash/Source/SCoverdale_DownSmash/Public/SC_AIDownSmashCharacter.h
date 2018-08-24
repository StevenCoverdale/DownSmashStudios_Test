// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCoverdale_DownSmashCharacter.h"
#include "SC_AIDownSmashCharacter.generated.h"



/**
 * 
 */
UCLASS()
class SCOVERDALE_DOWNSMASH_API ASC_AIDownSmashCharacter : public ASCoverdale_DownSmashCharacter
{
	GENERATED_BODY()

	ASC_AIDownSmashCharacter(const FObjectInitializer& ObjectInitializer);
public:


	UPROPERTY(EditAnywhere, Category = Behavior)
	class UBehaviorTree* BehaviorTree;

	FVector GetFinalDestination();

	virtual bool CanDealDamage_Implementation(UObject* FromObj = nullptr) override;

private:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface
	
};
