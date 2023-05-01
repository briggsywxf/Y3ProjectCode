// Fill out your copyright notice in the Description page of Project Settings.


#include "VRPuck.h"

AVRPuck::AVRPuck()
{
	// PuckMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	// PuckMesh->bUseDefaultCollision = true;
	// PuckMesh->SetSimulatePhysics(true);
}

void AVRPuck::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	PuckMesh->AddImpulse({-ReturnStrength * DeltaSeconds, 0.f, 0.f});
}

void AVRPuck::Respawn()
{
	PuckMesh->SetPhysicsLinearVelocity({0,0,0});
	PuckMesh->SetSimulatePhysics(false);
	// Get a random x offset of +- 10cm
	float XOffset = FMath::RandRange(-10.f, 10.f);
	auto SpawnTransform = MeshSpawnTransform;
	SpawnTransform.AddToTranslation({XOffset, 0.f, 0.f});
	PuckMesh->SetWorldTransform(SpawnTransform);
	PuckMesh->SetSimulatePhysics(true);

	// Add small random impulse to avoid being able to hit it straight into the goal
	PuckMesh->AddImpulse({0, FMath::RandRange(-40.f, 40.f), 0.f});
}
