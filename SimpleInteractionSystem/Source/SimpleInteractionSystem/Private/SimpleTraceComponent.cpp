// Copyright 2023 Georgios Lazaridis. All rights reserved.


#include "../Public/SimpleTraceComponent.h"
#include "../Public/SimpleTraceableComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Actor.h"

DEFINE_LOG_CATEGORY(LogSimpleInteractionSystem);

/**
 * Constructor for USimpleTraceComponent class.
 *
 * Sets the component to be initialized when the game starts and to be ticked every frame.
 */
USimpleTraceComponent::USimpleTraceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void USimpleTraceComponent::BeginPlay()
{
	Super::BeginPlay();
	
	PerformChecks();
	SetKeyBinds();
}

// Called every frame
void USimpleTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	TraceForObjects();
}

/**
 * @brief Perform the necessary checks for the SimpleTraceComponent.
 *
 * This method is called during the BeginPlay() method to perform checks on the owner's components.
 * It checks if the owner has a UCameraComponent with the specified CameraTag. If found, it sets the CameraComponent variable to the first found UCameraComponent.
 *
 * If no UCameraComponent is found with the specified CameraTag, an error message is logged.
 *
 * @note This method should be called during the BeginPlay() method to ensure correct initialization.
 * @note This method is specific to the SimpleTraceComponent class.
 */
void USimpleTraceComponent::PerformChecks()
{
	if(TArray<UActorComponent*> ComponentsWithTag = GetOwner()->GetComponentsByTag(UCameraComponent::StaticClass(), CameraTag); ComponentsWithTag.Num() > 0)
	{
		CameraComponent = Cast<UCameraComponent>(ComponentsWithTag[0]);
	}
	
	if(!CameraComponent)
	{
		UE_LOG(LogSimpleInteractionSystem, Error, TEXT("'%s' The owner does not contain a camera component with the correct tag. Trace will not work."), *GetNameSafe(this));
	}
}

/**
 * @brief Binds the specified key to the functions for button press and release.
 *
 * @param InteractionKey The key to bind for interaction.
 */
void USimpleTraceComponent::BindKeys(const FKey& InteractionKey)
{
	GetOwner()->InputComponent->BindKey(InteractionKey, IE_Pressed, this, &USimpleTraceComponent::OnButtonPressed);
	GetOwner()->InputComponent->BindKey(InteractionKey, IE_Released, this, &USimpleTraceComponent::OnButtonReleased);
}

/**
 * @brief Sets the key binds for the SimpleTraceComponent.
 *
 * This function binds the specified keyboard interaction key with the OnButtonPressed and OnButtonReleased functions.
 * If the `bEnableController` property is true, it also binds the controller interaction key with the same functions.
 */
void USimpleTraceComponent::SetKeyBinds()
{
	BindKeys(KeyboardInteractionKey);
	if(bEnableController)
	{
		BindKeys(ControllerInteractionKey);
	}
}

/**
 * @brief Performs a line trace for objects in the world.
 */
void USimpleTraceComponent::TraceForObjects()
{
	bool bHit = UKismetSystemLibrary::LineTraceSingleForObjects(GetOwner(), CameraComponent ? CameraComponent->GetComponentLocation() : GetOwner()->GetActorLocation(), GetEndTraceLocation(),
																LineTraceDetails.ObjectTypes, LineTraceDetails.bTraceComplex, LineTraceDetails.ActorsToIgnore,
																LineTraceDetails.DebugType, HitResult, LineTraceDetails.bIgnoreSelf,
																LineTraceDetails.TraceColor, LineTraceDetails.TraceHitColor, LineTraceDetails.DrawTime);

	
	if(bHit)
	{
		OnHit();
	}
	else
	{
		OnNoHit();
	}
}

/**
 * @brief Called when the object is hit by a line trace.
 *
 * This method checks if the SimpleTraceComponent has a SimpleTraceableComponent attached to it.
 * If it does, it checks the value of bCanTrace.
 * If bCanTrace is false, it calls the OnCantTrace() method.
 * If bCanTrace is true, it calls the OnCanTrace() method.
 */
void USimpleTraceComponent::OnHit()
{
	if(GetSimpleTraceableComponent())
	{
		if(bCanTrace == false)
		{
			OnCantTrace();
		}
		else
		{
			OnCanTrace();
		}
	}
}

/**
 * @brief Method called when the line trace does not hit any objects.
 *
 * If line trace is allowed (bCanTrace is true) and there is a valid CurrentComponentFromHit,
 * this method broadcasts the exit event and executes the exit delegate.
 *
 * @see USimpleTraceComponent::bCanTrace
 * @see USimpleTraceComponent::CurrentComponentFromHit
 * @see USimpleTraceComponent::BroadcastAndExecuteOnExit()
 */
void USimpleTraceComponent::OnNoHit()
{
	if(bCanTrace)
	{
		if(CurrentComponentFromHit)
		{
			BroadcastAndExecuteOnExit();
		}
	}
}


/**
 * @brief Handles the case when the trace cannot be performed on the current component
 *
 * This method checks if the current hit component is the same as the simple traceable component.
 * If it is, it calls the BroadcastAndExecuteOnHit method to trigger the necessary events.
 * This method is used in the OnHit method of the USimpleTraceComponent class.
 *
 * @see USimpleTraceComponent::OnHit
 * @see USimpleTraceComponent::BroadcastAndExecuteOnHit
 */
void USimpleTraceComponent::OnCantTrace()
{
	if(CurrentComponentFromHit){
		if(GetSimpleTraceableComponent() == CurrentComponentFromHit)
		{
			BroadcastAndExecuteOnHit();
		}
	}
}


/**
 * @brief Function called when the component is able to perform the trace.
 *
 * This function checks if there is a current component from hit. If there is, it compares it with the simple traceable component obtained from GetSimpleTraceableComponent().
 * If they are not equal, it calls BroadcastAndExecuteOnExit() function. Otherwise, it calls BroadcastAndExecuteOnHit() function.
 *
 * If there is no current component from hit, it assigns the value of GetSimpleTraceableComponent() to CurrentComponentFromHit and calls BroadcastAndExecuteOnHit() function.
 */
void USimpleTraceComponent::OnCanTrace()
{
	if(CurrentComponentFromHit)
	{
		if(GetSimpleTraceableComponent() != CurrentComponentFromHit)
		{
			BroadcastAndExecuteOnExit();
			
		}
		else
		{
			BroadcastAndExecuteOnHit();
		}
	}
	else
	{
		CurrentComponentFromHit = GetSimpleTraceableComponent();
		BroadcastAndExecuteOnHit();
	}
}

/**
 * @brief Broadcasts the hit event and calls the OnHit delegate.
 *
 * This method is called when the trace component hits a traceable component.
 * It broadcasts the hit event by calling the OnHitDel delegate and passes the
 * HitResult as a parameter. It then calls the Execute_OnHit function, passing
 * CurrentComponentFromHit and HitResult as parameters.
 *
 * @see OnHitDel
 * @see Execute_OnHit
 * @see HitResult
 * @see CurrentComponentFromHit
 */
void USimpleTraceComponent::BroadcastAndExecuteOnHit()
{
	OnHitDel.Broadcast(HitResult);
	Execute_OnHit(CurrentComponentFromHit, HitResult);
}

/**
 * @brief Broadcasts the OnExitDel delegate and executes the OnExit function.
 *
 * This method is called to broadcast the OnExitDel delegate, which can be bound to other functions or event listeners.
 * It also executes the OnExit function, passing the CurrentComponentFromHit as a parameter.
 *
 * @note This method sets the CurrentComponentFromHit to nullptr after execution.
 */
void USimpleTraceComponent::BroadcastAndExecuteOnExit()
{
	OnStopHitDel.Broadcast();
	Execute_OnStopHit(CurrentComponentFromHit);
	CurrentComponentFromHit = nullptr;
}

/**
 * @brief Handles the button press event.
 *
 * This method is called when the button is pressed.
 * It checks whether the current component from the hit result is valid and broadcasts the OnBeginInteractionDel delegate.
 * It also calls the Execute_OnBeginInteraction method for the current component from the hit result.
 * It sets the bCanTrace flag to false.
 *
 * @return void
 */
void USimpleTraceComponent::OnButtonPressed()
{
	if(CurrentComponentFromHit)
	{
		OnBeginInteractionDel.Broadcast();
		Execute_OnBeginInteraction(CurrentComponentFromHit);
		bCanTrace = false;
	}
}

/**
 * @brief This function is called when the button is released.
 *
 * If there is a current component from the hit, the function checks if tracing is allowed.
 * If tracing is not allowed, the OnEndInteractionDel and Execute_OnEndInteraction are broadcasted and executed, respectively.
 * The bCanTrace flag is set to true.
 *
 * @see OnEndInteractionDel, Execute_OnEndInteraction
 */
void USimpleTraceComponent::OnButtonReleased()
{
	if(CurrentComponentFromHit)
	{
		if(!bCanTrace)
		{
			OnEndInteractionDel.Broadcast();
			Execute_OnEndInteraction(CurrentComponentFromHit);
			bCanTrace = true;
		}
	}
}

/**
 * @return The end location of the trace
 */
FVector USimpleTraceComponent::GetEndTraceLocation() const
{
	return CameraComponent ? CameraComponent->GetComponentLocation() + CameraComponent->GetForwardVector() * LineTraceDetails.TraceDistance : GetOwner()->GetActorLocation();
}

/**
 * @return The SimpleTraceableComponent associated with the hit actor.
 */
USimpleTraceableComponent* USimpleTraceComponent::GetSimpleTraceableComponent() const
{
	return Cast<USimpleTraceableComponent>(HitResult.GetActor()->GetComponentByClass(USimpleTraceableComponent::StaticClass()));
}
