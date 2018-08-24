// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SCoverdale_DownSmashHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

ASCoverdale_DownSmashHUD::ASCoverdale_DownSmashHUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/FirstPerson/Textures/FirstPersonCrosshair"));
	CrosshairTex = CrosshairTexObj.Object;

	ConstructorHelpers::FClassFinder<UUserWidget> BPActionBar(TEXT("/Game/UI/WBP_ActionBar"));
	ActionBarClass = BPActionBar.Class;
}


void ASCoverdale_DownSmashHUD::DrawHUD()
{
	Super::DrawHUD();

	// Draw very simple crosshair

	// find center of the Canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
	const FVector2D CrosshairDrawPosition( (Center.X),
										   (Center.Y + 20.0f));

	// draw the crosshair
	FCanvasTileItem TileItem( CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem( TileItem );
}

void ASCoverdale_DownSmashHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();


	LoadActionBar();
}

void ASCoverdale_DownSmashHUD::LoadActionBar()
{
	if (!ensure(ActionBarClass != nullptr)) return;

	ActionBar = CreateWidget<UUserWidget>(GetOwningPlayerController(), ActionBarClass);
	if (!ensure(ActionBar != nullptr)) return;

	ActionBar->AddToViewport();
}
