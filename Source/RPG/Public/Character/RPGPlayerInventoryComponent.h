// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "Items/RPGEquipableItem.h"

#include "RPGPlayerInventoryComponent.generated.h"

class ARPGHeroCharacter;
/**
 * 
 */
UCLASS(meta = (BlueprintSpaenableComponent))
class RPG_API ARPGPlayerInventoryComponent : public AInfo
{
	GENERATED_BODY()
	public:

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




	ARPGPlayerInventoryComponent();

	void GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const override;

	UFUNCTION(BlueprintPure , Category= "RPG|InventoryComponent")
	int32 GetCurrentInventoryItemsNum() const;



	//Returns the items that are already equipped on the Player. 
	UFUNCTION(BlueprintPure , Category = "RPG|RPGPlayerState|Item")
    TArray<ARPGEquipableItem*> GetEquippedItems() const;


	/** Item Inventory Functions */

	/**Called when the Player Interacst with an item. Maybe the Item gets equipped or maybe it goes to the Inventory, it depends on the Character Weapon Mode 
	*
	* @param ItemToAdd Item that will be added to the Player.
	* @param Player The Player whom the item will be added.
	* @param SocketName The socket Name  which the Item will be attach to.
	*/
	UFUNCTION(BlueprintCallable , Category = "RPG|RPGPlayerState|Item")
    bool InteractWithItem(ARPGEquipableItem* Item , ARPGHeroCharacter* Player ,FName SocketName);



	

	protected:

	

	/** Inventory Properties */

	//Current Items Equipped by the Player.
	UPROPERTY( BlueprintReadOnly ,VisibleAnywhere, Category= "RPG|InventoryComopnent|Item", Replicated, EditFixedSize)
	TArray<ARPGEquipableItem*>EquippedItems;

	// This is the Weapon that the Character is holding on the right hand, could be a wand or a weapon.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere,Category = "RPG|InventoryComopnent|Item")
	ARPGEquipableItem* PrimaryItem;

	// This is the Secundary Item that the Character is holding on the left hand, could be a weapon, a shield or could be empty.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere,Category = "RPG|InventoryComopnent|Item")
    ARPGEquipableItem* SecundayItem;

	// This is the Secundary Item that the Character is holding on the left hand, could be a weapon, a shield or could be empty.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere,Category = "RPG|InventoryComopnent|Item")
    ARPGEquipableItem* BackPack;
	
	//Array that stores the data of all the items that are in the Inventory.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RPG|InventoryComopnent|Item" , Replicated, EditFixedSize)
    TArray<FRPGItemData>Inventory;

	//Player Inventory capacity.. Set Up to 3 by default, this can be changed on the future, or maybe depending on the character
	// I add some more amount to start the game
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category= "RPG|InventoryComopnent|Item", Replicated)
    int32 InventoryCapacity = 3;


protected:
	
	//Current Items Equipped by the Player.
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category= "RPG|InventoryComopnent|Item", Replicated)
    int32 CurrentInventoryItemsNum = 0;

	//Current Items Equipped by the Player.
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category= "RPG|InventoryComopnent|Item", Replicated)
    uint32 bAllPrimaryItemsEquipped :1;


	/** Inventory Functions */

	void ChangeItemEquipmentState(ARPGEquipableItem * Item, ARPGHeroCharacter* Player);

	/** Checks if the Item can be equipped or not on the Player. */
	bool CanEquipItem(ARPGEquipableItem* ItemToCheck,  ARPGHeroCharacter* Player);

	/** Equips the @ItemToAdd to the Player.*/
	void EquipItem(ARPGEquipableItem* ItemToAdd, ARPGHeroCharacter* Player, FName SocketName);

	/** Function that checks if the Item will go to the invetory of will be equipped, depending on the character`s WeaponMode.
	*For example, if We are in Doubble Sword Mode, if we take a shield, it will go to the inventory and it won`t be equipped.
	*@param ItemToCheck The item That will be checked for the interaction.
	*/
	bool CheckItemAnimationInteraction(ARPGEquipableItem* ItemToCheck, ARPGHeroCharacter* Player);
	
	/** Adds the @param ItemToAdd to the Inventory*/
	void AddItemToInventory(ARPGEquipableItem* ItemToAdd);

	/**We check if we cann add an item to the Bag
	* @return If the bag has capacity.
	*/
	bool CheckItemCapacity();


	/** Returs if the Player has the primary Items (Sword and shield, Double Sword... Equipped) */
	bool HasPlayerPrimaryItemsEquipped();

	void CheckAllPrimaryItemsEquipped(ARPGHeroCharacter* Player );

	//TODO Add the check for the item Equipment State ---> 1-> Primary Weapons 2-> BackPack + case 1.
	
	void SetItemVariables(ARPGEquipableItem* ItemToAdd , ARPGHeroCharacter* Player);
	
};
