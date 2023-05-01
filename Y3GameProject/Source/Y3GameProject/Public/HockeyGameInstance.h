// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "HockeyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class Y3GAMEPROJECT_API UHockeyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	void SetInitialPlayerTransform(FTransform NewTransform);
	FTransform GetInitialPlayerTransform() const { return InitialPlayerTransform;}

private:
	FTransform InitialPlayerTransform{};

	UPROPERTY()
	class UHockeySaveGame* SaveGameInstance;
};
