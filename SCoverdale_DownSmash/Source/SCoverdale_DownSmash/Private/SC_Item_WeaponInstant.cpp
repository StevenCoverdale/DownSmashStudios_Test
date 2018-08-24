// Fill out your copyright notice in the Description page of Project Settings.

#include "SC_Item_WeaponInstant.h"
#include "SCoverdale_DownSmashProjectile.h"
#include "SCoverdale_DownSmashCharacter.h"


ASC_Item_WeaponInstant::ASC_Item_WeaponInstant(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void ASC_Item_WeaponInstant::UseItem()
{
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			const FRotator SpawnRotation = OwningPawn->GetControlRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = OwningPawn->GetMuzzelLocation();

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// spawn the projectile at the muzzle
			auto proj = World->SpawnActor<ASCoverdale_DownSmashProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			if (proj)
				proj->SetOwner(OwningPawn);
		}
	}
	PlaySoundAndAnimation();
}

void ASC_Item_WeaponInstant::UpdateItem()
{

}

void ASC_Item_WeaponInstant::ItemSelection()
{

}

void ASC_Item_WeaponInstant::ItemDeselection()
{

}
