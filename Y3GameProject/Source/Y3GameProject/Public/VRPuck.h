// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePuck.h"
#include "VRPuck.generated.h"

/**
 * 
 */
UCLASS()
class Y3GAMEPROJECT_API AVRPuck : public ABasePuck
{
	GENERATED_BODY()
public:
	AVRPuck();

	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void Respawn() override;

	UPROPERTY(EditAnywhere)
	float ReturnStrength = 20.f;
	
};
