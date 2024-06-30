// Copyright 2023 Georgios Lazaridis. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SimpleComponent.h"
#include "SimpleTraceableComponent.generated.h"


UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class SIMPLEINTERACTIONSYSTEM_API USimpleTraceableComponent : public USimpleComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USimpleTraceableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit_Implementation(FHitResult& OutHit) override;
	virtual void OnStopHit_Implementation() override;
	virtual void OnBeginInteraction_Implementation() override;
	virtual void OnEndInteraction_Implementation() override;

	//Delegates
	UPROPERTY(BlueprintAssignable, Category="Delegates")
	FOnBeginInteractionDel OnBeginInteractionDel;
	
	UPROPERTY(BlueprintAssignable, Category="Delegates")
	FOnEndInteractionDel OnEndInteractionDel;

	UPROPERTY(BlueprintAssignable, Category="Delegates")
	FOnHitDel OnHitDel;
	
	UPROPERTY(BlueprintAssignable, Category="Delegates")
	FOnStopHitDel OnStopHitDel;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default", meta=(ToolTip="Use this tag in your static meshes in order to highlight them when traced. Highlight will affect every mesh with the tag even if they don't have the correct object type."))
	FName StaticMeshTag = "Highlight";

private:
	UPROPERTY()
	TArray<UStaticMeshComponent*> StaticMeshes;

	UPROPERTY()
	bool bIsHighlighted = false;

	void GetStaticMeshesWithTag();
	void SetRenderCustomDepthForAllStaticMeshes();
};
