// Copyright 2023 Georgios Lazaridis. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/HitResult.h"
#include "InputCoreTypes.h"
#include "SimpleComponent.h"
#include "SimpleTraceComponent.generated.h"

class UCameraComponent;
class USimpleTraceableComponent;

USTRUCT(BlueprintType)
struct FSimpleLineTrace
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Line Trace", meta=(ToolTip="The bigger the number, the greater the interaction distance."))
	double TraceDistance = 300;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Line Trace", meta=(ToolTip="You need to add at least one. It's highly recommended to create your own ObjectTypes."))
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Line Trace", meta=(ToolTip="Set to false if you dont eant to trace complex collision. Will save some resources but it will not be very acurate."))
	bool bTraceComplex = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Line Trace", meta=(ToolTip="No need to add self as it will ignored by default."))
	TArray<AActor*> ActorsToIgnore;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Line Trace", meta=(ToolTip="Debug line trace."))
	TEnumAsByte<EDrawDebugTrace::Type> DebugType = EDrawDebugTrace::None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Line Trace", meta=(ToolTip="Ignore self while tracing."))
	bool bIgnoreSelf = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Line Trace", meta=(EditConditionHides, EditCondition = "DebugType != EDrawDebugTrace::None", ToolTip="Debug line color."))
	FLinearColor TraceColor = FLinearColor::Red;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Line Trace", meta=(EditConditionHides, EditCondition = "DebugType != EDrawDebugTrace::None", ToolTip="Debug line hit color"))
	FLinearColor TraceHitColor = FLinearColor::Green;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Line Trace", meta=(EditConditionHides, EditCondition = "DebugType != EDrawDebugTrace::None", ToolTip="Debug line draw time duration."))
	float DrawTime = 5.0f;
	
};

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class SIMPLEINTERACTIONSYSTEM_API USimpleTraceComponent : public USimpleComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USimpleTraceComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Line Trace")
	FSimpleLineTrace LineTraceDetails;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Camera", meta=(ToolTip="Use this tag in your camera. Assuming you will only have one trace camera."))
	FName CameraTag = "TraceCamera";
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Interaction", meta=(ToolTip="Keyboard key for interaction."))
	FKey KeyboardInteractionKey = FKey(EKeys::F);
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Interaction", meta=(ToolTip="True if you want to enable controller keybind."))
	bool bEnableController = false;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Interaction", meta=(EditConditionHides, EditCondition = "bEnableController == true", ToolTip="Controller key for interaction."))
	FKey ControllerInteractionKey = FKey(EKeys::Gamepad_FaceButton_Top);
	
	//Delegates
	UPROPERTY(BlueprintAssignable, Category="Delegates")
	FOnBeginInteractionDel OnBeginInteractionDel;
	
	UPROPERTY(BlueprintAssignable, Category="Delegates")
	FOnEndInteractionDel OnEndInteractionDel;

	UPROPERTY(BlueprintAssignable, Category="Delegates")
	FOnHitDel OnHitDel;
	
	UPROPERTY(BlueprintAssignable, Category="Delegates")
	FOnStopHitDel OnStopHitDel;
	
private:
	
	UPROPERTY()
	UCameraComponent* CameraComponent = nullptr;

	UPROPERTY()
	USimpleTraceableComponent* CurrentComponentFromHit = nullptr;

	UPROPERTY()
	FHitResult HitResult;
	
	UPROPERTY()
	bool bCanTrace = true;

	void SetKeyBinds();
	void PerformChecks();
	void BindKeys(const FKey& InteractionKey);
	void OnButtonPressed();
	void OnButtonReleased();
	void OnHit();
	void OnNoHit();
	void BroadcastAndExecuteOnHit();
	void TraceForObjects();
	void OnCantTrace();
	void BroadcastAndExecuteOnExit();
	void OnCanTrace();
	USimpleTraceableComponent* GetSimpleTraceableComponent() const;
	FVector GetEndTraceLocation() const;
};
