// Copyright 2023 Georgios Lazaridis. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SimpleInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USimpleInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SIMPLEINTERACTIONSYSTEM_API ISimpleInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, Blueprintable, Category="Simple Interface")
	void OnHit(FHitResult& OutHit);

	UFUNCTION(BlueprintNativeEvent, Blueprintable, Category="Simple Interface")
	void OnStopHit();

	UFUNCTION(BlueprintNativeEvent, Blueprintable, Category="Simple Interface")
	void OnBeginInteraction();

	UFUNCTION(BlueprintNativeEvent, Blueprintable, Category="Simple Interface")
	void OnEndInteraction();
};
