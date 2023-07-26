// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "DimensionTriggerBox.generated.h"

/**
 * 
 */
UCLASS()
class GAEM_API ADimensionTriggerBox : public ATriggerBox
{
	GENERATED_BODY()
	
protected:

	// Called when the game starts or when spawned
	// virtual void BeginPlay() override;

public:

	// constructor sets default values for this actor's properties
	ADimensionTriggerBox();

    // declare overlap begin function
	UFUNCTION()
	void OnOverlapBegin(AActor* ThisActor, AActor* OtherActor);	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, meta = (AllowPrivateAccess = "true"))
	int32 DimensionNumber;
};
