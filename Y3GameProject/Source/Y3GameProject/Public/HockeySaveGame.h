// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "HockeySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class Y3GAMEPROJECT_API UHockeySaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FTransform SavedInitialPlayerTransform;
	
};
