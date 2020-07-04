// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RPGPlayerState.h"

#include "RPG/Public/Items/Weapons/RPGWeapon.h"



#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "AbilitySystem/RPGAttributeSetBase.h"
#include "Items/RPGBag.h"
#include "Net/UnrealNetwork.h"

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

    InventoryComponent = CreateDefaultSubobject<ARPGPlayerInventoryComponent>(FName("InventoryComponent"));


}

void ARPGPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
 
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

ARPGPlayerInventoryComponent* ARPGPlayerState::GetInventoryComponent()
{
  return IsValid(InventoryComponent) ? InventoryComponent : nullptr;
}

bool ARPGPlayerState::IsAlive() const
{
    return GetHealth()>0.0f;
}

ERPGTeam ARPGPlayerState::GetTeam() const
{
    return Team;
}












