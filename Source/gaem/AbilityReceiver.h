// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilityReceiver.generated.h"

/**
 * A component that allows the actor to be able to be manipulated using abilities
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAEM_API UAbilityReceiver : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAbilityReceiver();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	// virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, meta = (AllowPrivateAccess = "true"))
	int32 DimensionNumber;

	UFUNCTION(BlueprintCallable)
	void MoveToDimension(int32 NewDimensionNumber);
};
