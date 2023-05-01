// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HockeyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class Y3GAMEPROJECT_API AHockeyGameMode : public AGameModeBase
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerGoal, int, NewPlayerScore);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOpponentGoal, int, NewOpponentScore);

public:
	AHockeyGameMode();
	
	virtual void Tick(float DeltaSeconds) override;
	void PlayerGoal();
	void OpponentGoal();
	UFUNCTION(BlueprintCallable)
	float GetTimeRemaining() const { return TimeRemaining; }

	// Bonus target functions
	UFUNCTION(BlueprintCallable)
	void AddBonusTime(float BonusTime);
	UFUNCTION(BlueprintCallable)
	void AddBonusGoals(int BonusGoals);
	UFUNCTION(BlueprintCallable)
	void AddPenaltyGoals(int PenaltyGoals);

	UPROPERTY(BlueprintAssignable)
	FPlayerGoal OnPlayerGoal;
	UPROPERTY(BlueprintAssignable)
	FOpponentGoal OnOpponentGoal;

protected:
	virtual void BeginPlay() override;
	void GameEnd();
	
	float TimeRemaining = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	float TimeLimit = 120.f;
	bool GameOver = false;
	float BonusTimeAdded = 0.f;

	// Study stats
	int PlayerScore = 0;
	int OpponentScore = 0;
	int PlayerScoreBonusTime = 0;
	int OpponentScoreBonusTime = 0;
	float TimeToFirstGoal = 0.f;
	bool bBonusTimeTargetHit = false;
	bool bBonusGoalTargetHit = false;
	bool bPenaltyGoalTargetHit = false;
};
