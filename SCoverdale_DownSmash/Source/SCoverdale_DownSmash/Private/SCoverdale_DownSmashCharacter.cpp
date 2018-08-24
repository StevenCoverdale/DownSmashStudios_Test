// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SCoverdale_DownSmashCharacter.h"
#include "SCoverdale_DownSmashProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "MiscUtilities.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// ASCoverdale_DownSmashCharacter

ASCoverdale_DownSmashCharacter::ASCoverdale_DownSmashCharacter()
	: Super()
	, Health(100)
	, Gears(250)
	, BuildingDistance(500)
	, CellSize(10)
	, KillReward(25)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);
}

void ASCoverdale_DownSmashCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	SpawnDefaultItems();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	if(FP_Gun->MeshObject)
		FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
}

FVector ASCoverdale_DownSmashCharacter::GetMuzzelLocation()
{
	return FP_MuzzleLocation->GetComponentLocation();
}

float ASCoverdale_DownSmashCharacter::GetHealth()
{
	return Health;
}

bool ASCoverdale_DownSmashCharacter::IsAlive()
{
	return Health > 0;
}

void ASCoverdale_DownSmashCharacter::AddHealth(float Amount)
{
	Health += Amount;
	OnUpdateUI.Broadcast();


	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Pressed Key %d"), Health));
}

int ASCoverdale_DownSmashCharacter::GetGears()
{
	return Gears;
}

bool ASCoverdale_DownSmashCharacter::HasEnoughGears(int Cost)
{
	return Gears >= Cost;
}

void ASCoverdale_DownSmashCharacter::AddGears(int Amount)
{
	Gears += Amount;

	OnUpdateUI.Broadcast();

}

float ASCoverdale_DownSmashCharacter::GetBuildingDistance()
{
	return BuildingDistance;
}

float ASCoverdale_DownSmashCharacter::GetCellSize()
{
	return CellSize;
}

TArray<ASC_Item*> ASCoverdale_DownSmashCharacter::GetItems()
{
	return Items;
}

ASC_Item* ASCoverdale_DownSmashCharacter::GetCurrentItem()
{
	return CurrentItem;
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASCoverdale_DownSmashCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Action", IE_Pressed, this, &ASCoverdale_DownSmashCharacter::OnAction); //Triggers either or 

	//Bind Build Action event
	PlayerInputComponent->BindAction("Key1", IE_Pressed, this, &ASCoverdale_DownSmashCharacter::OnKey1);	//Weapon
	PlayerInputComponent->BindAction("Key2", IE_Pressed, this, &ASCoverdale_DownSmashCharacter::OnKey2);	//Blockade
	PlayerInputComponent->BindAction("Key3", IE_Pressed, this, &ASCoverdale_DownSmashCharacter::OnKey3);	//Spikes
	PlayerInputComponent->BindAction("Key4", IE_Pressed, this, &ASCoverdale_DownSmashCharacter::OnKey4);	//Tower
	
	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCoverdale_DownSmashCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCoverdale_DownSmashCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ASCoverdale_DownSmashCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ASCoverdale_DownSmashCharacter::LookUpAtRate);
}

void ASCoverdale_DownSmashCharacter::OnAction()
{
	if (CurrentItem && HasEnoughGears(CurrentItem->GetGearCost()))
	{
		OnStartAction();

	}
}

void ASCoverdale_DownSmashCharacter::OnStartAction()
{
	if (CurrentItem)
		CurrentItem->StartUsingItem();
}

void ASCoverdale_DownSmashCharacter::OnUpdateItem()
{
	if (CurrentItem)
		CurrentItem->StartUpdateItem();
}

void ASCoverdale_DownSmashCharacter::OnKey1()
{
	ChangeItemToIndex(0);
}

void ASCoverdale_DownSmashCharacter::OnKey2()
{
	ChangeItemToIndex(1);
}

void ASCoverdale_DownSmashCharacter::OnKey3()
{
	ChangeItemToIndex(2);
}

void ASCoverdale_DownSmashCharacter::OnKey4()
{
	ChangeItemToIndex(3);
}


void ASCoverdale_DownSmashCharacter::ChangeItemToIndex(int Index)
{
	if (CurrentItem)
		CurrentItem->OnItemDeselection();

	if (Items.IsValidIndex(Index))
	{
		CurrentItem = Items[Index];
		CurrentItem->OnItemSelection();
	}

	OnUpdateUI.Broadcast();


	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Pressed Key %d"), Index +1));
}

void ASCoverdale_DownSmashCharacter::SpawnDefaultItems()
{
	for (auto item : StartingItems)
	{
		FActorSpawnParameters spawnInfo;
		spawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ASC_Item* newItem = GetWorld()->SpawnActor<ASC_Item>(item, spawnInfo);
		if (newItem)
			AddItem(newItem);
	}
}

void ASCoverdale_DownSmashCharacter::AddItem(ASC_Item* InItem)
{
	if (InItem)
	{
		InItem->OwningPawn = this;
		Items.AddUnique(InItem);

		if (Items.Num() > 0 && CurrentItem == nullptr)
		{
			ChangeItemToIndex(0);
		}
	}
}

void ASCoverdale_DownSmashCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnAction();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void ASCoverdale_DownSmashCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

void ASCoverdale_DownSmashCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ASCoverdale_DownSmashCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ASCoverdale_DownSmashCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());

	OnUpdateItem();
}

void ASCoverdale_DownSmashCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());

	OnUpdateItem();
}

bool ASCoverdale_DownSmashCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ASCoverdale_DownSmashCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &ASCoverdale_DownSmashCharacter::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ASCoverdale_DownSmashCharacter::TouchUpdate);
		return true;
	}
	
	return false;
}

float ASCoverdale_DownSmashCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	AddHealth(-Damage);

	if (!IsAlive())
	{
		if (UMiscUtilities::DoesImplementInterface_T<UHasDamage>(DamageCauser) )
		{
			OnUnitKilled.Broadcast();
			IHasDamage::Execute_GiveKillReward(DamageCauser, KillReward);
			Destroy();
		}
	}
	return true;
}

bool ASCoverdale_DownSmashCharacter::CanDealDamage_Implementation(UObject* FromObj /*= nullptr*/)
{
	//If AI could Attack the player then we would allow it.
	return false;

	ASCoverdale_DownSmashProjectile* proj = Cast<ASCoverdale_DownSmashProjectile>(FromObj);

	if (IsAlive() && proj)
		return true;

	return false;
}

void ASCoverdale_DownSmashCharacter::GiveKillReward_Implementation(int Reward)
{
	AddGears(Reward);
}
