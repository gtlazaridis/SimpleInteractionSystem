// Copyright 2023 Georgios Lazaridis. All rights reserved.


#include "../Public/SimpleTraceableComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"


// Sets default values for this component's properties
USimpleTraceableComponent::USimpleTraceableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void USimpleTraceableComponent::BeginPlay()
{
	Super::BeginPlay();

	GetStaticMeshesWithTag();
}

// Called every frame
void USimpleTraceableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


/**
 * @brief Implementation of the OnHit_Implementation method.
 *
 * This method is called when an object is hit. It broadcasts the hit result through the OnHitDel delegate.
 * If the object is not already highlighted, it calls the SetRenderCustomDepthForAllStaticMeshes method
 * to set the render custom depth for all static meshes.
 *
 * @param OutHit The hit result containing information about the hit.
 */
void USimpleTraceableComponent::OnHit_Implementation(FHitResult& OutHit)
{
	OnHitDel.Broadcast(OutHit);
	if(!bIsHighlighted)
	{
		SetRenderCustomDepthForAllStaticMeshes();
	}
}

/**
 * @brief Called when exiting the traceable component.
 *
 * This function is called when exiting the traceable component and is responsible for broadcasting the OnStopHitDel delegate
 * and setting the render custom depth flag for all static meshes in the component.
 */
void USimpleTraceableComponent::OnStopHit_Implementation()
{
	OnStopHitDel.Broadcast();
	SetRenderCustomDepthForAllStaticMeshes();
}

/**
 * @brief Implementation of the OnBeginInteraction function for the USimpleTraceableComponent class.
 *
 * This function is called when the interaction begins. It broadcasts the OnBeginInteractionDel delegate.
 */
void USimpleTraceableComponent::OnBeginInteraction_Implementation()
{
	OnBeginInteractionDel.Broadcast();
}

/**
 * @brief Called when the interaction ends.
 *
 * This function is responsible for broadcasting the `OnEndInteractionDel` delegate, which can be used to notify other components or systems that the interaction has ended.
 *
 * @see OnEndInteractionDel
 */
void USimpleTraceableComponent::OnEndInteraction_Implementation()
{
	OnEndInteractionDel.Broadcast();
}

/**
 * @brief Get the static mesh components with the specified tag from the owner actor.
 */
void USimpleTraceableComponent::GetStaticMeshesWithTag()
{
	for(UActorComponent* Component : GetOwner()->GetComponentsByTag(UStaticMeshComponent::StaticClass(), StaticMeshTag))
	{
		if(UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(Component))
		{
			StaticMeshes.Add(StaticMeshComponent);
		}
	}
}

/**
 * @brief Sets whether to render custom depth for all static meshes.
 *
 * This method iterates over all the static meshes associated with this component
 * and toggles the render custom depth option based on the current state of the
 * 'bIsHighlighted' flag. This flag is then inverted.
 */
void USimpleTraceableComponent::SetRenderCustomDepthForAllStaticMeshes()
{
	for (const auto StaticMesh : StaticMeshes)
	{
		StaticMesh->SetRenderCustomDepth(!bIsHighlighted);
	}
	bIsHighlighted = !bIsHighlighted;
}
