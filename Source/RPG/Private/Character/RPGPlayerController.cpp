// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG/Public/Character/RPGPlayerController.h"
#include "Character/RPGPlayerController.h"
#include "Character/RPGPlayerState.h"


void ARPGPlayerController::CreateHUD()
{
    if (UIHUDWidget)
    {
        return;
    }

    if (!UIHUDWidgetClass)
    {
        UE_LOG(LogTemp , Error, TEXT("%s()  Missing from the blueprint properties. Please add the HUD class in order for the class to be created(Player Controller BP)"),*FString(__FUNCTION__));
        return;
    }


    //Only create the HUD for the local Player
    if (!IsLocalPlayerController())
    {
        return;
    }


    //We have to make sure the Player has a valid PS to get the attributes from

    ARPGPlayerState* PS = GetPlayerState<ARPGPlayerState>();

    if (!PS)
    {
        return;
    }


    UIHUDWidget = CreateWidget<URPGHUDWidget>(this,UIHUDWidgetClass);
    UIHUDWidget->AddToViewport();

    UIHUDWidget->SetCurrentHealth(PS->GetHealth());
    UIHUDWidget->SetMaxHealth(PS->GetMaxHealth());
    UIHUDWidget->SetHealthPercentage(PS->GetHealth() / PS->GetMaxHealth());


    DamageNumberClass = StaticLoadClass(UObject::StaticClass(),nullptr,TEXT("/Game/RPG/UI/WC_DamageText.WC_DamageText_C"));

    if (!DamageNumberClass)
    {
        UE_LOG(LogTemp,Error,TEXT(" %s() Failed to Find the Damage NUmberClass.If was moved, please update the reference location in the C++ class."),*FString(__FUNCTION__));
    }
    
    
}

URPGHUDWidget* ARPGPlayerController::GetHUD()
{
    return IsValid(UIHUDWidget) ? UIHUDWidget : nullptr;
}

void ARPGPlayerController::ShowDamaegNumber_Implementation(float DamageAmount, ARPGCharacterBase* TargetCharacter)
{
    URPGDamageTextWidgetComponent* DamageText = NewObject<URPGDamageTextWidgetComponent>(TargetCharacter, DamageNumberClass);
    DamageText->RegisterComponent();
    DamageText->AttachToComponent(TargetCharacter->GetRootComponent() , FAttachmentTransformRules::KeepRelativeTransform);
    DamageText->SetDamageText(DamageAmount);
    


    
}

bool ARPGPlayerController::ShowDamaegNumber_Validate(float DamageAmount, ARPGCharacterBase* TargetCharacter)
{
    return true;
}

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

    CreateHUD();
    
}
