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
        return AttributeSet->GetHealth();
}

float ARPGPlayerState::GetMaxHealth() const
{
        return AttributeSet->GetMaxHealth();
}

float ARPGPlayerState::GetMoveSpeed() const
{
        return AttributeSet->GetMoveSpeed();
}

float ARPGPlayerState::GetCharacterLevel() const
{
    if(AttributeSet)
    {
        AttributeSet->GetCharacterLevel();
    }

    return 0.0f;
}

bool ARPGPlayerState::IsAlive() const
{
   return GetHealth()>0.0f;
}


