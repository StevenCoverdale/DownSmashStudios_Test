// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MiscUtilities.generated.h"

/**
 * 
 */
UCLASS()
class SCOVERDALE_DOWNSMASH_API UMiscUtilities : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	template <typename T>
	static bool DoesImplementInterface_T(const UObject* Obj)
	{
		if (!Obj)
			return false;

		return Obj->GetClass()->ImplementsInterface(T::StaticClass());
	}
	
};
