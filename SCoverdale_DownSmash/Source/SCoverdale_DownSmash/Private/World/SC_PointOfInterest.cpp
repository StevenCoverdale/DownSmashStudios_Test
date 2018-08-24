// Fill out your copyright notice in the Description page of Project Settings.

#include "SC_PointOfInterest.h"
#include "IHasDamage.h"
#include "MiscUtilities.h"

// Sets default values
ASC_PointOfInterest::ASC_PointOfInterest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->SetupAttachment(RootComponent);
	BoxCollider->SetBoxExtent(FVector(50, 50, 50));
	BoxCollider->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

}

FVector ASC_PointOfInterest::GetEndPointLocation() const
{
	return EndGoal->GetActorLocation();
}

bool ASC_PointOfInterest::IsStartingPoint() const
{
	return bIsStartingPoint;
}

// Called when the game starts or when spawned
void ASC_PointOfInterest::BeginPlay()
{
	Super::BeginPlay();

	if (!IsStartingPoint())
	{
		BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ASC_PointOfInterest::OnOverlapStart);
	}
}

void ASC_PointOfInterest::OnOverlapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (UMiscUtilities::DoesImplementInterface_T<UHasDamage>(OtherActor) && IHasDamage::Execute_CanDealDamage(OtherActor, this))
	{
		LivesLost.Broadcast();
		OtherActor->Destroy();
	}
}

// Called every frame
void ASC_PointOfInterest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

