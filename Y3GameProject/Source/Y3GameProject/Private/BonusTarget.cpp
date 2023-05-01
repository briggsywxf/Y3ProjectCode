// Fill out your copyright notice in the Description page of Project Settings.


#include "BonusTarget.h"

#include "HockeyGameMode.h"
#include "Components/BillboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraComponent.h"

// Sets default values
ABonusTarget::ABonusTarget()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CapsuleComponent);

	// SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	// SphereMesh->SetupAttachment(RootComponent);
	ParticleSystemComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ParticleSystemComponent"));
	ParticleSystemComponent->SetupAttachment(RootComponent);

	IconBillboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("BillboardComponent"));
	IconBillboard->SetupAttachment(RootComponent);
	IconBillboard->SetHiddenInGame(false);
}

// Called when the game starts or when spawned
void ABonusTarget::BeginPlay()
{
	Super::BeginPlay();

	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent->SetVisibility(false, true);

	GameMode = Cast<AHockeyGameMode>(GetWorld()->GetAuthGameMode());
	check(GameMode);

	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ABonusTarget::OnTargetHit);

	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ABonusTarget::Spawn, TimeToSpawn, false);
}

void ABonusTarget::Despawn()
{
	Destroy();
}

void ABonusTarget::Spawn()
{
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RootComponent->SetVisibility(true, true);

	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ABonusTarget::Despawn, LifeTime, false);
}

// Called every frame
void ABonusTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
