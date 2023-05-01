// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePuck.generated.h"

UCLASS()
class Y3GAMEPROJECT_API ABasePuck : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasePuck();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Respawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USceneComponent* Root;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* PuckMesh;
	
	FTransform MeshSpawnTransform;
};
