// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "SC_AIController.generated.h"

/**
 * 
 */
UCLASS()
class SCOVERDALE_DOWNSMASH_API ASC_AIController : public AAIController
{
	GENERATED_BODY()

	ASC_AIController(const class FObjectInitializer& ObjectInitializer);
	
	virtual void Possess(class APawn* InPawn) override;

	virtual void UnPossess() override;

	//UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTreeComponent* BehaviorComp;

	//UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBlackboardComponent* BlackboardComp;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* BehaviorTree;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName EndGoal;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName PauseAI;

	void SetEndGoalLocation(FVector Location);
	void SetPauseAI(bool Pause);

	/** Returns BehaviorComp subobject **/
	FORCEINLINE UBehaviorTreeComponent* GetBehaviorComp() const { return BehaviorComp; }

	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }


	
};
