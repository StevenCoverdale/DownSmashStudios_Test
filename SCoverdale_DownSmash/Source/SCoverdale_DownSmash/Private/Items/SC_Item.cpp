// Fill out your copyright notice in the Description page of Project Settings.

#include "SC_Item.h"
#include "Kismet/GameplayStatics.h"
#include "SCoverdale_DownSmashCharacter.h"

ASC_Item::ASC_Item(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, GearCost(50)
{

}

int ASC_Item::GetGearCost()
{
	return GearCost;
}

void ASC_Item::StartUsingItem()
{
	UseItem();


}

void ASC_Item::InitializeItem(ASCoverdale_DownSmashCharacter* Pawn)
{
	OwningPawn = Pawn;
}

void ASC_Item::StartUpdateItem()
{
	UpdateItem();
}

void ASC_Item::OnItemSelection()
{
	ItemSelection();
}

void ASC_Item::OnItemDeselection()
{
	ItemDeselection();
}

void ASC_Item::PlaySoundAndAnimation()
{
	// try and play the sound if specified
	if (UseSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, UseSound, GetActorLocation());
	}

	//try and play a firing animation if specified
	if (UseAnimation != NULL)
	{
		//Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = OwningPawn->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(UseAnimation, 1.f);
		}
	}
}

// Called when the game starts or when spawned
void ASC_Item::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASC_Item::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

