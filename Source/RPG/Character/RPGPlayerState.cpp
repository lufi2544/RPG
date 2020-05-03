// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGPlayerState.h"



#include <xkeycheck.h>
#include <xkeycheck.h>
#include <xkeycheck.h>

ARPGPlayerState::ARPGPlayerState()
{
    AbilitySystemComponent = CreateDefaultSubobject<URPGAbilitySystemComponent>(TEXT("AbilitySystemComponent"));


       /**We set the Ability System to be replicated and also to be replicated in a way that the server
        will not tells us if anothe proxy has suffered a GE(Mixed Replication mode) .**/
    AbilitySystemComponent->SetIsReplicated(true);

    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    AttributeSet = CreateDefaultSubobject<URPGAttributeSetBase>(TEXT("AttributeSet"));

    // It is a must to update the components at the same frequency than the character.
    NetUpdateFrequency = 100.f;
    
}

UAbilitySystemComponent* ARPGPlayerState::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

URPGAttributeSetBase* ARPGPlayerState::GetAttributeSetBase() const
{
    return AttributeSet;   
}

float ARPGPlayerState::GetHealth() const
{
    if(AttributeSet)
    {
        return AttributeSet->GetHealth();
    }
    return 0.0f;
}

float ARPGPlayerState::GetMaxHealth() const
{
    if(AttributeSet)
    {
        return AttributeSet->GetMaxHealth();
    }
    return 0.0f;
}

float ARPGPlayerState::GetMoveSpeed() const
{
    if(AttributeSet)
    {
        return AttributeSet->GetMoveSpeed();
    }
    return 0.0f;
}

void ARPGPlayerState::PostInitializeComponents()
{
    Super::PostInitializeComponents();
}
