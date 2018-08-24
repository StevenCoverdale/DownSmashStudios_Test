// Fill out your copyright notice in the Description page of Project Settings.

#include "SC_AIController.h"
#include "SC_AIDownSmashCharacter.h"
#include "SCoverdale_DownSmashCharacter.h"




ASC_AIController::ASC_AIController(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	BehaviorComp = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	BlackboardComp = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	EndGoal = "Goal";
	PauseAI = "PauseAI";

	bWantsPlayerState = true;
}

void ASC_AIController::Possess(class APawn* InPawn)
{
	Super::Possess(InPawn);

	ASC_AIDownSmashCharacter* AIBot = Cast<ASC_AIDownSmashCharacter>(InPawn);
	if (AIBot)
	{
		if (AIBot->BehaviorTree->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*AIBot->BehaviorTree->BlackboardAsset);
		}

		BehaviorComp->StartTree(*AIBot->BehaviorTree);
	}

	//Find EndLocation and Set it as the destination
	SetEndGoalLocation(AIBot->GetFinalDestination());
}

void ASC_AIController::UnPossess()
{
	Super::UnPossess();

	//Stop any behavior running as we no longer have a pawn to control 
	BehaviorComp->StopTree();
}

void ASC_AIController::SetEndGoalLocation(FVector Location)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsVector(EndGoal, Location);
	}
}

void ASC_AIController::SetPauseAI(bool Pause)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsBool(PauseAI, Pause);
	}
}
