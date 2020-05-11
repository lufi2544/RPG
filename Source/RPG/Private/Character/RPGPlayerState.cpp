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

    bIsBackPackEquipped = false;

    bIsShieldEquipped = false;

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

bool ARPGPlayerState::IsAlive() const
{
    return GetHealth()>0.0f;
}

TArray<ARPGEquipableItem*> ARPGPlayerState::GetEquippedItems() const
{
    return  EquippedItems;
}


bool ARPGPlayerState::CheckItemAnimationInteraction(ARPGEquipableItem* ItemToCheck)
{
    bool bCanInteract = true;

    if(!bAllPrimaryItemsEquipped)
    {
        // This is the case when the item is a Weapon.
        if (ItemToCheck->GetItemData().ItemType == ERPGItemType::Weapon)
        {

        
            ARPGWeapon* Weapon = dynamic_cast<ARPGWeapon*>(ItemToCheck);

            if (Weapon)
            {
                if (Weapon->GetWeaponType() == ERPGWeaponType::Axe)
                {
            

                    CharacterAnimationMode = ERPGAnimationMode::AxeAndShield;

              
                }

                //We set a do once for the first sword and a then the second time the second sword will be equipped
                if ((Weapon->GetWeaponType() == ERPGWeaponType::Sword ))
                {
               
                    if( CharacterWeaponMode == ERPGCharacterWeaponMode::DoubleSword )
                    {
                                     
                        CharacterAnimationMode = ERPGAnimationMode::DoubleSword;
       
                    }else
                    {
   
                        CharacterAnimationMode = ERPGAnimationMode::SwordShield;
 
                    }
                }

                if (Weapon->GetWeaponType() == ERPGWeaponType::Bow)
                {
  
                    CharacterAnimationMode = ERPGAnimationMode::Bow;
                    
                }

                if (Weapon->GetWeaponType() == ERPGWeaponType::Wand)
                {
 
                    CharacterAnimationMode = ERPGAnimationMode::MagicWand;
                   
                }

            
                if (Weapon->GetWeaponType() == ERPGWeaponType::DoubleHandSword)
                {
    
                    CharacterAnimationMode = ERPGAnimationMode::DoubleHandSword;
                 
                }
            
            }
        }
    
   
        // This is the case when the item is a Shield.
        if (ItemToCheck->GetItemData().ItemType == ERPGItemType::Shield)
        {
            if( CharacterAnimationMode == ERPGAnimationMode::DoubleSword ||
            CharacterAnimationMode == ERPGAnimationMode::Bow || CharacterAnimationMode == ERPGAnimationMode::DoubleHandSword ||
            CharacterAnimationMode == ERPGAnimationMode::MagicWand)
            {
            
                AddItemToInventory(ItemToCheck);
                bCanInteract = false;
           
            
            }else
            {

                CharacterAnimationMode = ERPGAnimationMode::SwordShield;
     
            }
        }
    }
         // This is the case when the item is a BackPack.
    
        if (ItemToCheck->GetItemData().ItemType == ERPGItemType::BackPack)
        {
            ARPGBag* Bag = dynamic_cast<ARPGBag*>(ItemToCheck);

            // We Update the Capacity of our Inventory.
            InventoryCapacity = Bag->GetBagCapacity();
            
        }

    return bCanInteract;
}



bool ARPGPlayerState::CanEquipItem(ARPGEquipableItem* ItemToCheck)
{
     if(!IsValid(ItemToCheck) ||EquippedItems.Contains(ItemToCheck) ||
       ( ItemToCheck->GetItemData().ItemType == ERPGItemType::Weapon && bIsWeaponEquipped ) ||
       (ItemToCheck->GetItemData().ItemType == ERPGItemType::Shield && bIsShieldEquipped) ||
       (ItemToCheck->GetItemData().ItemType == ERPGItemType::BackPack && bIsBackPackEquipped))
     {
         UE_LOG(LogTemp , Error, TEXT("The item that the Player %s is trying to take, is not valid, it will go to the Inventory;") , *GetPlayerName());

       
         UE_LOG(LogTemp, Warning, TEXT("%f"),CurrentInventoryItemsNum);
         //The item addition to the Invetory (BackPack) will be handled via BP, go to the BP_ShieldBase for example and open the interface method to see it.
         
         return false;  

     }

    
     if(CheckItemAnimationInteraction(ItemToCheck))
     {
         ItemToCheck->SetIsEquipped(true);

         return  true;
     }
  
    
        return false;
}

void ARPGPlayerState::EquipItem(ARPGEquipableItem* ItemToAdd, ARPGCharacterBase* Player, FName SocketName)
{
    ItemToAdd->AttachToComponent(Player->GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,SocketName);

    ChangeItemEquipmentState(ItemToAdd);
}

bool ARPGPlayerState::InteractWithItem(ARPGEquipableItem* ItemToAdd,ARPGCharacterBase* Player , FName SocketName)
{
    if(!ensure(Player) || !ensure(ItemToAdd)){return false;}

    bool bSuccess = false;
    
    if(ItemToAdd && CanEquipItem(ItemToAdd))
    {
        EquippedItems.Add(ItemToAdd);

        if (!bAllPrimaryItemsEquipped && bIsSwordEquipped && CharacterAnimationMode == ERPGAnimationMode::DoubleSword)
        {
            //The Socket of the Second Sword has Reference directly to the RPG_Skeleton, if you change the left hand Socket Name, remember to change he name here too.
            EquipItem(ItemToAdd,Player,FName("hand_lShield"));

            ChangeItemEquipmentState(ItemToAdd);
            bSuccess = true;

           
        }else
        {
            EquipItem(ItemToAdd, Player, SocketName);
        
            bSuccess = true;
        }
    }else
    {
        // This means that we have to add the item to the Inventory

        AddItemToInventory(ItemToAdd);
    }
    
    return bSuccess;
}

void ARPGPlayerState::AddItemToInventory(ARPGEquipableItem* ItemToAdd)
{
    if(IsValid(ItemToAdd) && CheckItemCapacity())
    {
        
        //We add the item data to the Inventory to store it.
        Inventory.Add(ItemToAdd->GetItemData());
        
        
        ItemToAdd->Destroy();

        
    }
}

bool ARPGPlayerState::CheckItemCapacity()
{
    if (CurrentInventoryItemsNum < InventoryCapacity)
    {
        CurrentInventoryItemsNum++;
        return true;
    }
    UE_LOG(LogTemp, Error, TEXT("INVENTORY FULL!!!!, Current Items: %i    , Inventory Capacity : %i"), CurrentInventoryItemsNum,InventoryCapacity);
    return false;


}

void ARPGPlayerState::ChangeItemEquipmentState(ARPGEquipableItem* Item)
{
    if ( Item->GetItemData().ItemType == ERPGItemType::Weapon )     
    {
        ARPGWeapon* Weapon = dynamic_cast<ARPGWeapon*>(Item);

         if (Weapon)
            {
                if (Weapon->GetWeaponType() == ERPGWeaponType::Axe)
                {
                    bIsAxeEquipped = true;
                    bIsWeaponEquipped = true;

                }

                //We set a do once for the first sword and a then the second time the second sword will be equipped
                if ((Weapon->GetWeaponType() == ERPGWeaponType::Sword ))
                {
               
                    if( CharacterWeaponMode == ERPGCharacterWeaponMode::DoubleSword )
                    {
                        if(bIsSwordEquipped)
                        {
                            bIsWeaponEquipped = true;
                            bIsDoubbleSwordEquipped = true;
        
                        }

                        bIsSwordEquipped = true;

                    }else
                    {
                        bIsSwordEquipped = true;
                        bIsWeaponEquipped = true;

                    }
                }

                if (Weapon->GetWeaponType() == ERPGWeaponType::Bow)
                {
                    bIsBowEquipped = true;
                    bIsWeaponEquipped = true;
              


                    
                }

                if (Weapon->GetWeaponType() == ERPGWeaponType::Wand)
                {
                    bIsWandEquipped = true;
                    bIsWeaponEquipped = true;
               


                   
                }

            
                if (Weapon->GetWeaponType() == ERPGWeaponType::DoubleHandSword)
                {
                    bIsDoubbleHandSwordEquipped = true;
                    bIsWeaponEquipped = true;
               


                 
                }
            
            }
        }

    if (Item->GetItemData().ItemType == ERPGItemType::BackPack)
    {
        ARPGBag* Bag = dynamic_cast<ARPGBag*>(Item);

        bIsBackPackEquipped = true;
            
    }

    if (Item->GetItemData().ItemType == ERPGItemType::Shield)
    {
       bIsShieldEquipped = true;
    }

}

bool ARPGPlayerState::HasPlayerPrimaryItemsEquipped()
{
    if (bIsDoubbleSwordEquipped || (bIsShieldEquipped && bIsSwordEquipped) || (bIsWandEquipped)
        || (bIsAxeEquipped && bIsShieldEquipped) || bIsBowEquipped || bIsDoubbleHandSwordEquipped)
    {
        bAllPrimaryItemsEquipped = true;
    }

    return bAllPrimaryItemsEquipped;
}








