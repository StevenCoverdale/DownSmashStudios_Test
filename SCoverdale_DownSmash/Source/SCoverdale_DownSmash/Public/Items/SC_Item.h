// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/Texture2D.h"
#include "SC_Item.generated.h"

UENUM()
enum class EItemState : uint8
{
	Idle,
	Using,
};

UCLASS()
class SCOVERDALE_DOWNSMASH_API ASC_Item : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASC_Item(const FObjectInitializer& ObjectInitializer);

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class USoundBase* UseSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* UseAnimation;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Item|Info")
	FText DisplayName;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Item|Info")
	UTexture2D* ItemsTexture;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	int GearCost;

	/** pawn owner */
	UPROPERTY()
	class ASCoverdale_DownSmashCharacter* OwningPawn;
	UFUNCTION(BlueprintCallable, Category = "Item|Info")
	int GetGearCost();
	void StartUsingItem();
	void InitializeItem(ASCoverdale_DownSmashCharacter* Pawn);

	void StartUpdateItem();

	virtual void OnItemSelection();
	virtual void OnItemDeselection();

	void PlaySoundAndAnimation();

protected:


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void ItemSelection() PURE_VIRTUAL(ASC_Item::ItemSelection, );

	virtual void ItemDeselection() PURE_VIRTUAL(ASC_Item::ItemDeselection, );

	virtual void UpdateItem() PURE_VIRTUAL(ASC_Item::UpdateItem, );

	virtual void UseItem() PURE_VIRTUAL(ASC_Item::UseItem, );

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
