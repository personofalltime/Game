// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilityReceiver.h"
#include "DimensionTriggerBox.h"

ADimensionTriggerBox::ADimensionTriggerBox() {
    OnActorBeginOverlap.AddDynamic(this, &ADimensionTriggerBox::OnOverlapBegin);
}

void ADimensionTriggerBox::OnOverlapBegin(AActor* ThisActor, AActor* OtherActor) {
    // This is needed to make sure that the actors are placed in the correct dimension when loaded in    
    // Automatically set the correct dimension for the actors that enter the trigger
    // See if the other actor has an ability receiver
    if (UAbilityReceiver* abilityReceiver = OtherActor->FindComponentByClass<UAbilityReceiver>()){
        // Make sure that the ability receiver's dimension is updated
        abilityReceiver->DimensionNumber = DimensionNumber;
    }
}