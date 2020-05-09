// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RPGPlayerState.h"

#include <xkeycheck.h>



#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "AbilitySystem/RPGAttributeSetBase.h"
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
    
}

void ARPGPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    
}

bool ARPGPlayerState::EquipItem(ARPGEquipableItem* ItemToAdd,ARPGCharacterBase* Player , FName SocketName)
{
    if(!ensure(Player) || !ensure(ItemToAdd)){return false;}

    bool bSuccess = false;
    
    if(ItemToAdd && CanEquipItem(ItemToAdd))
    {
        EquippedItems.Add(ItemToAdd);

        ItemToAdd->AttachToComponent(Player->GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,SocketName);
        
        bSuccess = true;
    }
    
    return bSuccess;
}

bool ARPGPlayerState::CanEquipItem(ARPGEquipableItem* ItemToCheck)
{
     if(!IsValid(ItemToCheck) ||EquippedItems.Contains(ItemToCheck) ||
       ( ItemToCheck->GetItemData().ItemType == ERPGItemType::Weapon && bIsWeaponEquipped ) ||
       (ItemToCheck->GetItemData().ItemType == ERPGItemType::Shield && bIsShieldEquipped) ||
       (ItemToCheck->GetItemData().ItemType == ERPGItemType::BackPack && bIsBackPackEquipped))
     {
         UE_LOG(LogTemp , Error, TEXT("The item that the Player %s is trying to take, is not valid;") , *GetPlayerName());


         return false;  

     }
    
     if (ItemToCheck->GetItemData().ItemType == ERPGItemType::Weapon)
     {
         bIsWeaponEquipped = true;
         bIsSwordEquipped = true;
     }
    
     if (ItemToCheck->GetItemData().ItemType == ERPGItemType::Shield)
     {
         bIsShieldEquipped = true;
     }
    
     if (ItemToCheck->GetItemData().ItemType == ERPGItemType::BackPack)
      {
          bIsBackPackEquipped = true;
      }

        return  true;
}

void ARPGPlayerState::AddItemToInventory(ARPGEquipableItem* ItemToAdd)
{
    if(IsValid(ItemToAdd))
    {
        //InventoryItemsData.Add(ItemToAdd->GetItemData());

        ItemToAdd->Destroy();

        
    }
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

TArray<ARPGEquipableItem*> ARPGPlayerState::GetEquippedItems() const
{
    return  EquippedItems;
}



