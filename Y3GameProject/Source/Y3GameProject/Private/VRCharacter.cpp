// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacter.h"

#include "MotionControllerComponent.h"
#include "MRPuck.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HockeyGameInstance.h"

// Sets default values
AVRCharacter::AVRCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VRRoot"));
	VRRoot->SetupAttachment(RootComponent);
	VRCameraRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VRCameraRoot"));
	VRCameraRoot->SetupAttachment(VRRoot);
	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	VRCamera->SetupAttachment(VRCameraRoot);

	LeftControllerRoot = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftControllerRoot"));
	LeftControllerRoot->SetupAttachment(VRRoot);
	ControllerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ControllerMesh"));
	ControllerMesh->SetupAttachment(LeftControllerRoot);
	ControllerCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ControllerCollision"));
	ControllerCollision->SetupAttachment(LeftControllerRoot);

}

// Called when the game starts or when spawned
void AVRCharacter::BeginPlay()
{
	Super::BeginPlay();

	auto GameInstance = Cast<UHockeyGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		FTransform InitialTransform = GameInstance->GetInitialPlayerTransform();
		SetActorLocation({InitialTransform.GetLocation().X, InitialTransform.GetLocation().Y, GetActorLocation().Z});
		VRRoot->SetRelativeLocation({0.f, 0.f, InitialTransform.GetLocation().Z});
		VRRoot->SetRelativeRotation(InitialTransform.GetRotation());

		UE_LOG(LogTemp, Warning, TEXT("VRC Setting actor world offset: x: %f, y: %f"), InitialTransform.GetLocation().X, InitialTransform.GetLocation().Y);
	}
	
}

// Called every frame
void AVRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AVRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("X-Axis", this, &AVRCharacter::MoveY);
	PlayerInputComponent->BindAxis("Y-Axis", this, &AVRCharacter::MoveX);
	PlayerInputComponent->BindAxis("Rotate", this, &AVRCharacter::Rotate);

	PlayerInputComponent->BindAxis("Move Up", this, &AVRCharacter::MoveUp);
	PlayerInputComponent->BindAxis("Move Down", this, &AVRCharacter::MoveDown);

	PlayerInputComponent->BindAction("Resend Start", IE_Pressed, this, &AVRCharacter::ResendStart);

	PlayerInputComponent->BindAction("Save Transform", IE_Pressed, this, &AVRCharacter::SaveTransform);

}

void AVRCharacter::MoveUp(float Val)
{
	VRRoot->AddLocalOffset({0, 0, Val * 0.01});
}

void AVRCharacter::MoveDown(float Val)
{
	VRRoot->AddLocalOffset({0, 0, -Val * 0.01});
}

void AVRCharacter::ResendStart()
{
	auto Puck = Cast<AMRPuck>(UGameplayStatics::GetActorOfClass(this, AMRPuck::StaticClass()));
	if (Puck)
	{
		Puck->SendRequest(1);
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Puck found!"));
	}
}

void AVRCharacter::SaveTransform()
{
	// UE_LOG(LogTemp, Warning, TEXT("Actor world offset: x: %f, y: %f"), GetActorLocation().X, GetActorLocation().Y);
	// UE_LOG(LogTemp, Warning, TEXT("VRRoot local offset: z: %f"), VRRoot->GetRelativeLocation().Z);
	// UE_LOG(LogTemp, Warning, TEXT("VRRoot yaw: %f"), VRRoot->GetComponentRotation().Yaw);

	FTransform NewTransform;
	NewTransform.SetLocation({GetActorLocation().X, GetActorLocation().Y, VRRoot->GetRelativeLocation().Z});
	NewTransform.SetRotation(VRRoot->GetComponentRotation().Quaternion());

	auto GameInstance = Cast<UHockeyGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->SetInitialPlayerTransform(NewTransform);
	}
}

void AVRCharacter::MoveX(float Val)
{
	AddActorWorldOffset({Val * 0.1, 0, 0});
}

void AVRCharacter::MoveY(float Val)
{
	AddActorWorldOffset({0, Val * 0.1, 0});
}

void AVRCharacter::Rotate(float Val)
{
	VRRoot->AddWorldRotation({0, Val * 0.1, 0});
}

