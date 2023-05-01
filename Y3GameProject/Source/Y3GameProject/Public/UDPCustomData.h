#pragma once

#include "Containers/UnrealString.h"
#include "UDPCustomData.generated.h"

USTRUCT(BlueprintType)
struct FUDPCustomData
{
	GENERATED_BODY()

	// UPROPERTY()
	char RequestType = 0;

	// UPROPERTY()
	// float xCoord = 0.f;
	//
	// float yCoord = 0.f;

	FUDPCustomData() {}
	
};

FORCEINLINE FArchive& operator<<(FArchive &Ar, FUDPCustomData& DataStruct)
{
	Ar << DataStruct.RequestType;
	// if (DataStruct.RequestType == 2)
	// {
	// 	Ar << DataStruct.xCoord;
	// 	Ar << DataStruct.yCoord;
	// }

	// DataStruct.RequestData.SerializeAsANSICharArray(Ar, DataStruct.RequestData.Len());

	return Ar;
}