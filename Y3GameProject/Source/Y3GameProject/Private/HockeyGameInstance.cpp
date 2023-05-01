// Fill out your copyright notice in the Description page of Project Settings.


#include "HockeyGameInstance.h"

#include "HockeySaveGame.h"
#include "Kismet/GameplayStatics.h"

void UHockeyGameInstance::Init()
{
	Super::Init();

	// Load game save from slot
	SaveGameInstance = Cast<UHockeySaveGame>(UGameplayStatics::LoadGameFromSlot("HockeySaveGame", 0));

	// Load saved transform into local variable
	if (SaveGameInstance)
	{
		InitialPlayerTransform = SaveGameInstance->SavedInitialPlayerTransform;
		UE_LOG(LogTemp, Warning, TEXT("Loaded save successfully"))
		UE_LOG(LogTemp, Warning, TEXT("HGI Loading Actor world offset: x: %f, y: %f"), InitialPlayerTransform.GetLocation().X, InitialPlayerTransform.GetLocation().Y);
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not load save, creating new one"))
		SaveGameInstance = Cast<UHockeySaveGame>(UGameplayStatics::CreateSaveGameObject(UHockeySaveGame::StaticClass()));
		bool bSaved = UGameplayStatics::SaveGameToSlot(SaveGameInstance, "HockeySaveGame", 0);
		if (bSaved) UE_LOG(LogTemp, Warning, TEXT("Saved new save game instance"))
	}
}

void UHockeyGameInstance::SetInitialPlayerTransform(FTransform NewTransform)
{
	InitialPlayerTransform = NewTransform;

	if (SaveGameInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("HGI Saving new transform"))
		UE_LOG(LogTemp, Warning, TEXT("HGI Saving Actor world offset: x: %f, y: %f"), NewTransform.GetLocation().X, NewTransform.GetLocation().Y);
		UE_LOG(LogTemp, Warning, TEXT("HGI Saving VRRoot local offset: z: %f"), NewTransform.GetLocation().Z);
		UE_LOG(LogTemp, Warning, TEXT("HGI Saving VRRoot rot Z: %f"), NewTransform.GetRotation().Z);
		// Save transform to save game instance
		SaveGameInstance->SavedInitialPlayerTransform = InitialPlayerTransform;
		// Save save game instance to slot
		bool bSaved = UGameplayStatics::SaveGameToSlot(SaveGameInstance, "HockeySaveGame", 0);
		if (bSaved) UE_LOG(LogTemp, Warning, TEXT("Saved successfully"))
	}
}

