// Fill out your copyright notice in the Description page of Project Settings.

#include "SC_BuildingBase.h"
#include "IHasDamage.h"
#include "MiscUtilities.h"
#include "NavAreas/NavArea.h"

// Sets default values
ASC_BuildingBase::ASC_BuildingBase()
	: Super()
	, Damage(20)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a gun mesh component
	Root = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));

	Root->SetupAttachment(RootComponent);

	// Create a gun mesh component
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->bCastDynamicShadow = true;
	BaseMesh->CastShadow = true;
	BaseMesh->SetupAttachment(Root);
	BaseMesh->SetCollisionResponseToAllChannels(ECR_Overlap);
	BaseMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	BaseMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
}

void ASC_BuildingBase::OnOverlapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(CanPawnBeAttacked(OtherActor))
	{
		SetTarget(OtherActor);
		OnAttackStarted();
	}
}

void ASC_BuildingBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

bool ASC_BuildingBase::HasOverlappingBuildings()
{
	TArray<AActor*> overlapping;
	auto Baseclass = GetClass();
	GetOverlappingActors(overlapping, ASC_BuildingBase::StaticClass());

	if (overlapping.Num() > 0)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(2, 0.1f, FColor::Red, FString::Printf(TEXT("Overlapping")));
	}

	return overlapping.Num() > 0;
}

bool ASC_BuildingBase::CanPawnBeAttacked(AActor* Target)
{
	if (UMiscUtilities::DoesImplementInterface_T<UHasDamage>(Target) && IHasDamage::Execute_CanDealDamage(Target, this))
	{
		return true;
	}
	return false;
}

TArray<AActor*> ASC_BuildingBase::GetOverlappingPawns()
{
	TArray<AActor*> retVal;

	return retVal;
}

void ASC_BuildingBase::PlaceBuilding()
{
	TimeBetweenShots = 60.0f / ShotsPerMinute;
	ToggleBlueprintCollision(true);
}

void ASC_BuildingBase::OnAttackStarted()
{
	if (GetOverlappingPawns().Num() > 0)
	{
		// Start firing, can be delayed to satisfy TimeBetweenShots
		const float GameTime = GetWorld()->GetTimeSeconds();
		if (LastFireTime > 0 && TimeBetweenShots > 0.0f &&
			LastFireTime + TimeBetweenShots > GameTime)
		{
			GetWorldTimerManager().SetTimer(TimerHandle_HandleFiring, this, &ASC_BuildingBase::HandleAttack, LastFireTime + TimeBetweenShots - GameTime, false);
		}
		else
		{
			HandleAttack();
		}
	}
}

AActor* ASC_BuildingBase::GetTarget()
{
	return CurrentTarget;
}

void ASC_BuildingBase::SetTarget(AActor* Target)
{
	CurrentTarget = Target;
}

bool ASC_BuildingBase::ApplyDamage(float InDamage)
{
	const auto pawns = GetOverlappingPawns();
	bool appliedDamage = false;
	for (auto pawn : pawns)
	{
		FPointDamageEvent PointDmg;
		PointDmg.ShotDirection = GetActorUpVector();

		PointDmg.Damage = InDamage;

		if (pawn && CanPawnBeAttacked(pawn))
		{
			pawn->TakeDamage(InDamage, PointDmg, nullptr, OwningPawn);
			appliedDamage = true;
		}
	}

	OnAttackStarted();
	return appliedDamage;
}

void ASC_BuildingBase::HandleAttack()
{
	LastFireTime = GetWorld()->GetTimeSeconds();
}

// Called when the game starts or when spawned
void ASC_BuildingBase::BeginPlay()
{
	Super::BeginPlay();	

	ToggleBlueprintCollision(false);

}

// Called every frame
void ASC_BuildingBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

