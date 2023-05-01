// Fill out your copyright notice in the Description page of Project Settings.


#include "HockeyGameMode.h"

AHockeyGameMode::AHockeyGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHockeyGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!GameOver)
	{
		TimeRemaining -= DeltaSeconds;
	}

	if (TimeRemaining < 0 && !GameOver)
	{
		GameEnd();
	}
}

void AHockeyGameMode::PlayerGoal()
{
	if (!GameOver)
	{
		// If first goal, record time
		if (PlayerScore == 0)
		{
			TimeToFirstGoal = TimeLimit - TimeRemaining;
		}
		OnPlayerGoal.Broadcast(++PlayerScore);
		UE_LOG(LogTemp, Warning, TEXT("Player scored"))
		// If scored in bonus time, record separately
		if (TimeRemaining <= BonusTimeAdded)
		{
			++PlayerScoreBonusTime;
		}
	}
}

void AHockeyGameMode::OpponentGoal()
{
	if (!GameOver)
	{
		OnOpponentGoal.Broadcast(++OpponentScore);
		UE_LOG(LogTemp, Warning, TEXT("Opponent scored"))
		// If scored in bonus time, record separately
		if (TimeRemaining <= BonusTimeAdded)
		{
			++OpponentScoreBonusTime;
		}
	}
}

void AHockeyGameMode::AddBonusTime(const float BonusTime)
{
	if (!GameOver)
	{
		TimeRemaining += BonusTime;
		BonusTimeAdded += BonusTime;
		bBonusTimeTargetHit = true;
	}
}

void AHockeyGameMode::AddBonusGoals(const int BonusGoals)
{
	if (!GameOver)
	{
		PlayerScore += BonusGoals;
		bBonusGoalTargetHit = true;
		UE_LOG(LogTemp, Warning, TEXT("%d bonus goals added"), BonusGoals)
		OnPlayerGoal.Broadcast(PlayerScore);
	}
}

void AHockeyGameMode::AddPenaltyGoals(const int PenaltyGoals)
{
	if (!GameOver)
	{
		OpponentScore += PenaltyGoals;
		bPenaltyGoalTargetHit = true;
		UE_LOG(LogTemp, Warning, TEXT("%d penalty goals removed"), PenaltyGoals)
		OnOpponentGoal.Broadcast(OpponentScore);
	}
}

void AHockeyGameMode::BeginPlay()
{
	Super::BeginPlay();

	TimeRemaining = TimeLimit;
}

void AHockeyGameMode::GameEnd()
{
	GameOver = true;
	UE_LOG(LogTemp, Warning, TEXT("Time's up!"));
	UE_LOG(LogTemp, Warning, TEXT("------------ RESULTS ------------"))
	UE_LOG(LogTemp, Warning, TEXT("Player Score: %d"), PlayerScore);
	UE_LOG(LogTemp, Warning, TEXT("Opponent Score: %d"), OpponentScore);
	UE_LOG(LogTemp, Warning, TEXT("Time to first goal: %f"), TimeToFirstGoal);
	UE_LOG(LogTemp, Warning, TEXT("Player goals in bonus time: %d"), PlayerScoreBonusTime);
	UE_LOG(LogTemp, Warning, TEXT("Opponent goals in bonus time: %d"), OpponentScoreBonusTime);
	UE_LOG(LogTemp, Warning, TEXT("Bonus time target hit: %d"), bBonusTimeTargetHit);
	UE_LOG(LogTemp, Warning, TEXT("Bonus goal target hit: %d"), bBonusGoalTargetHit);
	UE_LOG(LogTemp, Warning, TEXT("Penalty goal target hit: %d"), bPenaltyGoalTargetHit);
	UE_LOG(LogTemp, Warning, TEXT("---------------------------------"))
}
