// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "RPGCharacterBase.h"
#include "RPG/Public/Items/RPGEquipableItem.h"
#include "RPG/RPG.h"



#include "RPGPlayerState.generated.h"

class ARPGCharacterBase;
class URPGAbilitySystemComponent;
class URPGAttributeSetBase;

/**
 * 
 */
UCLASS()
class RPG_API ARPGPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

		ARPGPlayerState();
	
	//Getter for the AbilitySystemComponent from the Interface
	class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const override;
	
	//Getter for the AttributeSet Base
	class URPGAttributeSetBase* GetAttributeSetBase() const;

	/*
	 * Getters for the Character State(AttributeSet related)
	 */

	UFUNCTION(BlueprintPure , Category = "RPG|RPGPlayerState|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintPure , Category = "RPG|RPGPlayerState|Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintPure , Category = "RPG|RPGPlayerState|Attributes")
	float GetMoveSpeed() const;

	UFUNCTION(BlueprintPure , Category = "RPG|RPGPlayerState|Attributes")
	float GetCharacterLevel() const;

	UFUNCTION(BlueprintPure , Category = "RPG|RPGPlayerState|State")
	bool IsAlive() const;

	//Returns the items that are already equipped on the Player. 
	UFUNCTION(BlueprintPure , Category = "RPG|RPGPlayerState|Item")
	TArray<ARPGEquipableItem*> GetEquippedItems() const;

	
	
	/** Item Inventory Functions */

	/** This function will Equip the Item to the Player if possible. If the player can`t equip the item because he has already some items equipped, they will be placed on the Inventory.
	 *Also checks depending on the Current Character Weapon Mode if the Item could be equipped(If are in Double Sword Mode, we can`t grab a shield and equip it, it will go to the Inventory)
	 *The function checks if we can equipp the item, if there is already a weapon equipped, the item will go to the inventory.
	 * @param ItemToAdd Item that will be added to the Player.
	 * @param Player The Player whom the item will be added.
	 * @param SocketName The socket Name  which the Item will be attach to.
	 */
	UFUNCTION(BlueprintCallable , Category = "RPG|RPGPlayerState|Item")
	bool EquipItem(ARPGEquipableItem* ItemToAdd , ARPGCharacterBase* Player ,FName SocketName);

	


protected:

	/** Variables for the Inventory and in hand Items Logic */
	uint32 bIsWeaponEquipped : 1;
	uint32 bIsDoubbleSwordEquipped :1;
	uint32 bIsSwordEquipped : 1;
	uint32 bIsShieldEquipped : 1;
	uint32 bIsDoubbleHandSwordEquipped : 1;
	uint32 bIsAxeEquipped : 1;
	uint32 bIsWandEquipped : 1;
	uint32 bIsBowEquipped : 1;
	uint32 bIsBackPackEquipped : 1;

	UPROPERTY()
	class URPGAbilitySystemComponent* AbilitySystemComponent;
	
	UPROPERTY()
	class URPGAttributeSetBase* AttributeSet;

	/** Animation State Variables */

	UPROPERTY(BlueprintReadWrite, EditAnywhere , Category= "RPG|RPGPlayerState|Animation" , Replicated)
	ERPGCharacterWeaponMode CharacterWeaponMode = ERPGCharacterWeaponMode::NoWeapon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere , Category= "RPG|RPGPlayerState|Animation" , Replicated)
	ERPGAnimationMode CharacterAnimationMode = ERPGAnimationMode::NoWeapon;


	/** Inventory Properties */

	//Current Items Equipped by the Player.
	UPROPERTY( BlueprintReadOnly ,VisibleAnywhere, Category= "RPG|RPGPlayerState|Item", Replicated, EditFixedSize)
    TArray<ARPGEquipableItem*>EquippedItems;
	
	//Array that stores the data of all the items that are in the Inventory.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RPG|RPGPlayerState" , Replicated, EditFixedSize)
    TArray<struct FRPGItemData>Inventory;

	//Player Inventory capacity..
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category= "RPG|RPGPlayerState|Item", Replicated)
    int32 InventoryCapacity = 3;

	//Current Items Equipped by the Player.
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category= "RPG|RPGPlayerState|Item", Replicated)
    int32 CurrentInventoryItemsNum = 0;

	//Current Items Equipped by the Player.
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category= "RPG|RPGPlayerState|Item", Replicated)
	uint32 bAllPrimaryItemsEquipped :1;

	

	

	/** Inventory Functions */

	void ChangeItemEquipmentState(ARPGEquipableItem * Item);

	/** Checks if the Item can be equipped or not on the Player. */
	bool CanEquipItem(ARPGEquipableItem* ItemToCheck);

	/** Function that checks if the Item will go to the invetory of will be equipped, depending on the character`s WeaponMode.
	*For example, if We are in Doubble Sword Mode, if we take a shield, it will go to the inventory and it won`t be equipped.
	*@param ItemToCheck The item That will be checked for the interaction.
	*/
	bool CheckItemAnimationInteraction(ARPGEquipableItem* ItemToCheck);
	
	/** Adds the @param ItemToAdd to the Inventory*/
	void AddItemToInventory(ARPGEquipableItem* ItemToAdd);

	/**We check if we cann add an item to the Bag
	 * @return If the bag has capacity.
	 */
	bool CheckItemCapacity();


	/** Returs if the Player has the primary Items (Sword and shield, Double Sword... Equipped) */
	bool HasPlayerPrimaryItemsEquipped();

	
};


