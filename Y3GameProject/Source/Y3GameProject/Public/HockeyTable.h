// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HockeyTable.generated.h"

UCLASS()
class Y3GAMEPROJECT_API AHockeyTable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHockeyTable();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void PlayerScored(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitResult);
	UFUNCTION()
	void OpponentScored(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitResult);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* TableMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UBoxComponent* PlayerGoal;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UBoxComponent* OpponentGoal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UWidgetComponent* StatsWidget;

	UPROPERTY()
	class AHockeyGameMode* GameMode;

};
