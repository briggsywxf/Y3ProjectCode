// Fill out your copyright notice in the Description page of Project Settings.


#include "HockeyTable.h"

#include "BasePuck.h"
#include "HockeyGameMode.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
AHockeyTable::AHockeyTable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TableMesh"));
	SetRootComponent(TableMesh);

	PlayerGoal = CreateDefaultSubobject<UBoxComponent>(TEXT("PlayerGoal"));
	PlayerGoal->SetupAttachment(RootComponent);

	OpponentGoal = CreateDefaultSubobject<UBoxComponent>(TEXT("OpponentGoal"));
	OpponentGoal->SetupAttachment(RootComponent);

	StatsWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("StatsWidget"));
	StatsWidget->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AHockeyTable::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<AHockeyGameMode>(GetWorld()->GetAuthGameMode());
	check(GameMode);

	OpponentGoal->OnComponentBeginOverlap.AddDynamic(this, &AHockeyTable::PlayerScored);
	PlayerGoal->OnComponentBeginOverlap.AddDynamic(this, &AHockeyTable::OpponentScored);
	
}

void AHockeyTable::PlayerScored(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitResult)
{
	auto Puck = Cast<ABasePuck>(OtherActor);
	if (Puck)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player scored!"))
		GameMode->PlayerGoal();
		Puck->Respawn();
	}
}

void AHockeyTable::OpponentScored(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitResult)
{
	auto Puck = Cast<ABasePuck>(OtherActor);
	if (Puck)
	{
		UE_LOG(LogTemp, Warning, TEXT("Opponent scored!"))
		GameMode->OpponentGoal();
		Puck->Respawn();
	}
}

// Called every frame
void AHockeyTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

