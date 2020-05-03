// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG/AbilitySystem/RPGAttributeSetBase.h"

#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "RPG/Character/RPGPlayerController.h"

//We set the base values on the Player Attribute GameplayEffect
URPGAttributeSetBase::URPGAttributeSetBase()
{
}

void URPGAttributeSetBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSetBase, Health, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSetBase, MaxHealth, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSetBase, MoveSpeed, COND_None, REPNOTIFY_Always);
    
}

void URPGAttributeSetBase::OnRep_Health()
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSetBase , Health);
}

void URPGAttributeSetBase::OnRep_MaxHealth()
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSetBase , MaxHealth);
}
void URPGAttributeSetBase::OnRep_MoveSpeed()
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSetBase , MoveSpeed);
}




void URPGAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{

   Super::PostGameplayEffectExecute(Data);

   
}
