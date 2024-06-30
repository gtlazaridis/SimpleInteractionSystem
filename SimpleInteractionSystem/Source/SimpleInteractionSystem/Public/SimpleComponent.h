// Copyright 2023 Georgios Lazaridis. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SimpleInterface.h"
#include "Components/ActorComponent.h"
#include "Engine/HitResult.h"
#include "SimpleComponent.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(LogSimpleInteractionSystem, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndInteractionDel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBeginInteractionDel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopHitDel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHitDel, FHitResult, OutHit);

UCLASS(ClassGroup=(SimpleInteractionSystem))
class SIMPLEINTERACTIONSYSTEM_API USimpleComponent : public UActorComponent, public ISimpleInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USimpleComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
