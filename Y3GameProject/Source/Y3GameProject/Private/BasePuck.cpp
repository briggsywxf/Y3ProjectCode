// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePuck.h"

// Sets default values
ABasePuck::ABasePuck()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	PuckMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PuckMesh"));
	PuckMesh->SetupAttachment(Root);

}

// Called when the game starts or when spawned
void ABasePuck::BeginPlay()
{
	Super::BeginPlay();

	MeshSpawnTransform = PuckMesh->GetComponentTransform();
	
}

// Called every frame
void ABasePuck::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABasePuck::Respawn()
{
	UE_LOG(LogTemp, Warning, TEXT("Puck should override respawn method!"));
	return;
}

