// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BonusTarget.generated.h"

UCLASS()
class Y3GAMEPROJECT_API ABonusTarget : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABonusTarget();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Despawn();
	void Spawn();

	UFUNCTION(BlueprintImplementableEvent)
	void OnTargetHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitResult);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UCapsuleComponent* CapsuleComponent;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly)
	// UStaticMeshComponent* SphereMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UNiagaraComponent* ParticleSystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UBillboardComponent* IconBillboard;

	UPROPERTY(BlueprintReadOnly)
	class AHockeyGameMode* GameMode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TimeToSpawn = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LifeTime = 5.f;

	FTimerHandle SpawnTimerHandle;

};
