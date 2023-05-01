// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePuck.h"
#include "MRPuck.generated.h"

/**
 * 
 */
UCLASS()
class Y3GAMEPROJECT_API AMRPuck : public ABasePuck
{
	GENERATED_BODY()

public:
	AMRPuck();

	virtual void Tick(float DeltaTime) override;

	bool StartUDPSender();
	bool SendRequest(char RequestType);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	
	TSharedPtr<FInternetAddr> RemoteAddr;
	
	FSocket* UDPSocket;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString SocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString ServerIP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ServerPort = 25555;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 GamePort = 25554;

	bool IsUDP;

	FTransform PuckOffset{};

	TPair<float, float> FrameN1{0.f, 0.f};
	TPair<float, float> FrameN2{0.f, 0.f};
	
};
