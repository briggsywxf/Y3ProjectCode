// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VRCharacter.generated.h"

UCLASS()
class Y3GAMEPROJECT_API AVRCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVRCharacter();

protected:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void MoveX(float Val);
	void MoveY(float Val);
	void Rotate(float Val);
	void MoveUp(float Val);
	void MoveDown(float Val);
	void ResendStart();
	void SaveTransform();
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USceneComponent* VRRoot;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USceneComponent* VRCameraRoot;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UCameraComponent* VRCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UMotionControllerComponent* LeftControllerRoot;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ControllerMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCapsuleComponent* ControllerCollision;

};
