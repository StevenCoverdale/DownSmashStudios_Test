// Fill out your copyright notice in the Description page of Project Settings.

#include "SC_BuildingBase_Trap.h"
#include "DrawDebugHelpers.h"
#include "SC_AIDownSmashCharacter.h"



ASC_BuildingBase_Trap::ASC_BuildingBase_Trap()
	: Super()
	, MaxRange(500)
{

	// Create a gun mesh component
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->SetupAttachment(BaseMesh);
	BoxCollider->SetBoxExtent(FVector(50, 50, 50));
	BoxCollider->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	//BoxCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

}


void ASC_BuildingBase_Trap::PlaceBuilding()
{
	Super::PlaceBuilding();

	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ASC_BuildingBase::OnOverlapStart);
	BoxCollider->OnComponentEndOverlap.AddDynamic(this, &ASC_BuildingBase::OnOverlapEnd);

	SetCollisionLocation();
}



FHitResult ASC_BuildingBase_Trap::DoTrace(const FVector& Start, const FVector& End)
{
	FCollisionQueryParams TraceParams(TEXT("Trace"), true, Instigator);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;
	TraceParams.AddIgnoredActor(this);


	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_Vehicle, TraceParams);

	return Hit;
}

void ASC_BuildingBase_Trap::SetCollisionLocation()
{
	const FVector& startingPos = GetActorLocation();
	const FVector& endpos = startingPos + (GetActorUpVector() * MaxRange);
	const auto& hit = DoTrace(startingPos, endpos);
	/*DrawDebugLine(
		GetWorld(),
		startingPos,
		endpos,
		FColor(0, 255, 0),
		false, 60, 0,
		12.333
	);*/
	const float& maxBoxExtentVal = BoxCollider->GetUnscaledBoxExtent().GetMax();
	BoxCollider->SetWorldScale3D(BaseMesh->GetComponentScale() + (FVector(0, 0, (hit.Distance == 0 ? MaxRange : hit.Distance) / 2) / maxBoxExtentVal));

	const float& maxScaleVal = BoxCollider->RelativeScale3D.GetMax();
	const FVector& newLocation = BoxCollider->GetRelativeTransform().GetLocation() + (FVector(0, 0, maxScaleVal * maxBoxExtentVal));
	BoxCollider->SetRelativeLocation(newLocation);
}

void ASC_BuildingBase_Trap::BeginPlay()
{

}

void ASC_BuildingBase_Trap::HandleAttack()
{
	Super::HandleAttack();

	auto target = GetTarget();
	if (target)
		ApplyDamage(Damage);

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Attacking")));
}

TArray<AActor*> ASC_BuildingBase_Trap::GetOverlappingPawns()
{
	TArray<AActor*> overlapping;
	auto Baseclass = GetClass();
	GetOverlappingActors(overlapping, ASC_AIDownSmashCharacter::StaticClass());

	return overlapping;
}
