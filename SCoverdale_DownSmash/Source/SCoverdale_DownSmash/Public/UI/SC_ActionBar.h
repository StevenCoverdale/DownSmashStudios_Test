// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SC_ActionBar.generated.h"

/**
 * 
 */
UCLASS()
class SCOVERDALE_DOWNSMASH_API USC_ActionBar : public UUserWidget
{
	GENERATED_BODY()


	
	
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	
};
