// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SCoverdale_DownSmashHUD.generated.h"

UCLASS()
class ASCoverdale_DownSmashHUD : public AHUD
{
	GENERATED_BODY()

public:
	ASCoverdale_DownSmashHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;
	virtual void PostInitializeComponents() override;

	void LoadActionBar();

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;


	TSubclassOf<class UUserWidget> ActionBarClass;
	UUserWidget* ActionBar;

};

