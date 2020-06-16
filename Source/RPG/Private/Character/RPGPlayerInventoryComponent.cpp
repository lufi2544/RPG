// Fill out your copyright notice in the Description page of Project Settings.

#include "RPG/Public/Character/RPGPlayerInventoryComponent.h"


#include "Character/RPGPlayerState.h"
#include "RPG/Public/Items/Weapons/RPGWeapon.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "AbilitySystem/RPGAttributeSetBase.h"
#include "Items/RPGBag.h"
#include "Net/UnrealNetwork.h"


#include "Character/RPGHeroCharacter.h"

ARPGPlayerInventoryComponent::ARPGPlayerInventoryComponent()
{

    
}

void ARPGPlayerInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{

    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

int32 ARPGPlayerInventoryComponent::GetCurrentInventoryItemsNum() const
{
    return CurrentInventoryItemsNum;
}

TArray<ARPGEquipableItem*> ARPGPlayerInventoryComponent::GetEquippedItems() const
{
    return EquippedItems;
}

bool ARPGPlayerInventoryComponent::InteractWithItem(ARPGEquipableItem* Item, ARPGHeroCharacter* Player, FName SocketName)
{
    if(!ensure(Player) || !ensure(Item)){return false;}

    bool bSuccess = true;
    
    if(Item && CanEquipItem(Item,Player) && !HasPlayerPrimaryItemsEquipped())
    {
        
        EquipItem(Item,Player,SocketName);
       
    }else
    {
        // This means that we have to add the item to the Inventory

        AddItemToInventory(Item);
        bSuccess = false;
    }
    
    return bSuccess;
}

void ARPGPlayerInventoryComponent::ChangeItemEquipmentState(ARPGEquipableItem* Item , ARPGHeroCharacter* Player)
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
               
                    if( Player->GetCharacterHeroType() == ERPGCharacterHeroType::Indrax )
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

    // If We have already the Primary Items Equipped, it is not necessary to Execute the Function
     if (!PrimaryItem || !SecundayItem || !BackPack)
     {
         SetItemVariables(Item,Player);
     }
   

}

bool ARPGPlayerInventoryComponent::CanEquipItem(ARPGEquipableItem* ItemToCheck,  ARPGHeroCharacter* Player)
{
    if(!IsValid(ItemToCheck) ||EquippedItems.Contains(ItemToCheck) ||
      ( ItemToCheck->GetItemData().ItemType == ERPGItemType::Weapon && bIsWeaponEquipped ) ||
      (ItemToCheck->GetItemData().ItemType == ERPGItemType::Shield && bIsShieldEquipped) ||
      (ItemToCheck->GetItemData().ItemType == ERPGItemType::BackPack && bIsBackPackEquipped))
    {
       

       
        UE_LOG(LogTemp, Warning, TEXT("%f"),CurrentInventoryItemsNum);
        //The item addition to the Invetory (BackPack) will be handled via BP, go to the BP_ShieldBase for example and open the interface method to see it.
         
        return false;  

    }

    
    if(CheckItemAnimationInteraction(ItemToCheck,Player))
    {
        ItemToCheck->SetIsEquipped(true);

        return  true;
    }
  
    
    return false;
}

void ARPGPlayerInventoryComponent::EquipItem(ARPGEquipableItem* ItemToAdd, ARPGHeroCharacter* Player, FName SocketName)
{
    //Second Sword Attachment
    if ( bIsSwordEquipped && Player->GetCharacterAnimationMode() == ERPGAnimationMode::DoubleSword && ItemToAdd->GetItemData().ItemType != ERPGItemType::BackPack )
    {
        //The Socket of the Second Sword has Reference directly to the RPG_Skeleton, if you change the left hand Socket Name, remember to change he name here too.
        ItemToAdd->AttachToComponent(Player->GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,FName("hand_lShield"));        

        ChangeItemEquipmentState(ItemToAdd,Player);
          
    }else
    {
        ItemToAdd->AttachToComponent(Player->GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,SocketName);
        
        ChangeItemEquipmentState(ItemToAdd,Player);      
    }
    
    EquippedItems.Add(ItemToAdd);
}

bool ARPGPlayerInventoryComponent::CheckItemAnimationInteraction(ARPGEquipableItem* ItemToCheck,ARPGHeroCharacter* Player)
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
            

                    Player->SetCharacterAnimationMode(ERPGAnimationMode::AxeAndShield);

              
                }

                //We set a do once for the first sword and a then the second time the second sword will be equipped
                if ((Weapon->GetWeaponType() == ERPGWeaponType::Sword ))
                {
               
                    if( Player->GetCharacterHeroType() == ERPGCharacterHeroType::Indrax )
                    {
                                     
                       Player->SetCharacterAnimationMode( ERPGAnimationMode::DoubleSword);
       
                    }else
                    {
   
                         Player->SetCharacterAnimationMode(ERPGAnimationMode::SwordShield);
 
                    }
                }

                if (Weapon->GetWeaponType() == ERPGWeaponType::Bow)
                {
  
                     Player->SetCharacterAnimationMode(ERPGAnimationMode::Bow); 
                    
                }

                if (Weapon->GetWeaponType() == ERPGWeaponType::Wand)
                {
 
                     Player->SetCharacterAnimationMode(ERPGAnimationMode::MagicWand);
                   
                }

            
                if (Weapon->GetWeaponType() == ERPGWeaponType::DoubleHandSword)
                {
    
                    Player->SetCharacterAnimationMode(ERPGAnimationMode::DoubleHandSword);
                 
                }
            
            }
        }
    
   
        // This is the case when the item is a Shield.
        if (ItemToCheck->GetItemData().ItemType == ERPGItemType::Shield)
        {
            if( (Player->GetCharacterHeroType() == (ERPGCharacterHeroType::Spinder) ||
                   ( Player->GetCharacterHeroType() == ERPGCharacterHeroType::Kevalam)))          
            {
            
                Player->SetCharacterAnimationMode(ERPGAnimationMode::SwordShield);
           
            
            }else
            {

                

                AddItemToInventory(ItemToCheck);
                bCanInteract = false;
     
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

void ARPGPlayerInventoryComponent::AddItemToInventory(ARPGEquipableItem* ItemToAdd)
{
    if(IsValid(ItemToAdd) && CheckItemCapacity())
    {
        
        //We add the item data to the Inventory to store it.
        Inventory.Add(ItemToAdd->GetItemData());
        
        
        ItemToAdd->Destroy();

        
    }
}

bool ARPGPlayerInventoryComponent::CheckItemCapacity()
{
    if (CurrentInventoryItemsNum < InventoryCapacity)
    {
        CurrentInventoryItemsNum++;
        return true;
    }
    UE_LOG(LogTemp, Error, TEXT("INVENTORY FULL!!!!, Current Items: %i    , Inventory Capacity : %i"), CurrentInventoryItemsNum,InventoryCapacity);
    return false;

}

bool ARPGPlayerInventoryComponent::HasPlayerPrimaryItemsEquipped()
{
    if ((bIsDoubbleSwordEquipped || (bIsShieldEquipped && bIsSwordEquipped) || (bIsWandEquipped)
       || (bIsAxeEquipped && bIsShieldEquipped) || bIsBowEquipped || bIsDoubbleHandSwordEquipped) && bIsBackPackEquipped)
    {
        bAllPrimaryItemsEquipped = true;
    }

    return bAllPrimaryItemsEquipped;
}

void ARPGPlayerInventoryComponent::SetItemVariables(ARPGEquipableItem* ItemToAdd, ARPGHeroCharacter* Player)
{
    if (!PrimaryItem && ItemToAdd)
    {

        if ( Player->GetCharacterHeroType() == ERPGCharacterHeroType::Indrax && bIsSwordEquipped )
        {
            PrimaryItem = ItemToAdd;
        }

        if ( bIsWeaponEquipped  )
        {
            PrimaryItem = ItemToAdd;  
        }
    }

    if ( !SecundayItem && ItemToAdd )
    {
        if (bIsDoubbleSwordEquipped || bIsShieldEquipped)
        {
            SecundayItem = ItemToAdd;
        }
    }

}

