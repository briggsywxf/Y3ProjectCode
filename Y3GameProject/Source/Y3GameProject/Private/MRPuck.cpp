// Fill out your copyright notice in the Description page of Project Settings.


#include "MRPuck.h"

#include "DetailLayoutBuilder.h"
#include "Networking.h"
#include "UDPCustomData.h"

AMRPuck::AMRPuck()
{
	PrimaryActorTick.bCanEverTick = true;

	UDPSocket = nullptr;
}

void AMRPuck::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	int32 BufferSize = 2 * 1024 * 1024;
	uint8* Buffer = new uint8[BufferSize];
	int32 BytesRead = 0;

	UDPSocket->Recv(Buffer, BufferSize, BytesRead);

	// TODO: Interpolation / prediction if no packet?

	if (BytesRead != 0)
	{
		char PacketType;
		memcpy(&PacketType, Buffer, sizeof(char));
		if (PacketType == 3)
		{
			float XPos, YPos;
			memcpy(&XPos, &Buffer[1], sizeof(float));
			memcpy(&YPos, &Buffer[1 + sizeof(float)], sizeof(float));

			// UE_LOG(LogTemp, Warning, TEXT("XPos: %f, YPos: %f"), XPos, YPos);
			// if (GEngine)
			// {
			// 	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red,
			// 	                                 FString::Printf(TEXT("XPos: %f, YPos: %f"), XPos, YPos));
			// }
			FTransform Transform = PuckMesh->GetComponentTransform();
			Transform.SetRotation({0,0,0,0});
			Transform.SetLocation({XPos * 100, YPos * 100, 1.f});
			PuckMesh->SetRelativeTransform(Transform);
			// FrameN2 = FrameN1;
			// FrameN1 = {XPos, YPos};
			// SetActorTransform(Transform);
		}
	}
	// else
	// {
	// 	// Interpolate based on previous 2 frames
	// 	FTransform Transform = PuckMesh->GetComponentTransform();
	// 	// Get difference between frames
	// 	float XDiff = FrameN2.Key - FrameN1.Key;
	// 	float YDiff = FrameN2.Value - FrameN1.Value;
	//
	// 	// Position is N1 plus diff
	// 	float XPos = FrameN1.Key + XDiff;
	// 	float YPos = FrameN1.Value + YDiff;
	//
	// 	Transform.SetLocation({XPos * 100, YPos * 100, 1.f});
	// 	PuckMesh->SetRelativeTransform(Transform);
	// 	FrameN2 = FrameN1;
	// 	FrameN1 = {XPos, YPos};
	// }

	delete[] Buffer;
}

bool AMRPuck::StartUDPSender()
{
	RemoteAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	bool bIsValid;
	RemoteAddr->SetIp(*ServerIP, bIsValid);
	RemoteAddr->SetPort(ServerPort);

	if (!bIsValid)
	{
		UE_LOG(LogTemp, Warning, TEXT("IP Address Invalid"));
		return false;
	}

	FIPv4Endpoint LocalEndpoint{FIPv4Address{192, 168, 0, 66}, static_cast<uint16>(GamePort)};
	UDPSocket = FUdpSocketBuilder(*SocketName).AsReusable().AsNonBlocking().BoundToEndpoint(LocalEndpoint);

	int32 SendSize = 2 * 1024 * 1024;
	UDPSocket->SetSendBufferSize(SendSize, SendSize);
	UDPSocket->SetReceiveBufferSize(SendSize, SendSize);

	UE_LOG(LogTemp, Warning, TEXT("UDP sender initialized successfully"))

	return true;
}

void AMRPuck::BeginPlay()
{
	Super::BeginPlay();

	StartUDPSender();

	// Ask server to start sending tracking data
	SendRequest(1);
}

void AMRPuck::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UDPSocket->Close();
}

bool AMRPuck::SendRequest(char RequestType)
{
	if (!UDPSocket)
	{
		UE_LOG(LogTemp, Warning, TEXT("No sender socket"));
		return false;
	}

	int32 BytesSent = 0;

	FUDPCustomData PacketData;
	PacketData.RequestType = RequestType;

	FArrayWriter Writer;

	Writer << PacketData;

	UDPSocket->SendTo(Writer.GetData(), Writer.Num(), BytesSent, *RemoteAddr);

	if (BytesSent <= 0)
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("Socket valid but receiver received 0 bytes. Make sure it is listening properly"));
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("UDP Send success"));

	return true;
}
