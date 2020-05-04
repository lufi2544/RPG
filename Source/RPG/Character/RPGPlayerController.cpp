// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGPlayerController.h"

#include "RPGPlayerState.h"

void ARPGPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    ARPGPlayerState* PS = GetPlayerState<ARPGPlayerState>();

    if(PS)
    {
        PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS,InPawn);
    }
    
}

void ARPGPlayerController::OnRep_PlayerState()
{

    Super::OnRep_PlayerState();


    
}
