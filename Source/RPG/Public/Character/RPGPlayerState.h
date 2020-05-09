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
	


	/** This function attaches the curret item to the Player himself.
	 *The function checks if we can equipp the item, if there is already a weapon equipped, the item will go to the inventory.
	 * @param ItemToAdd Item that will be added to the Player.
	 * @param Player The Player whom the item will be added.
	 * @param SocketName The socket Name  which the Item will be attach to.
	 */
	UFUNCTION(BlueprintCallable , Category = "RPG|RPGPlayerState|Item")
	bool EquipItem(ARPGEquipableItem* ItemToAdd , ARPGCharacterBase* Player ,FName SocketName);

	
	

protected:

	uint32 bIsWeaponEquipped : 1;
	
	uint32 bIsSwordEquipped : 1;
	uint32 bIsShieldEquipped : 1;
	uint32 bIsDoubblehandSwordEquipped : 1;
	uint32 bIsWandEquipped : 1;
	uint32 bIsBowEquipped : 1;
	uint32 bIsBackPackEquipped : 1;

	UPROPERTY()
	class URPGAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	class URPGAttributeSetBase* AttributeSet;

	//Current Items Equipped by the Player.
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category= "RPG|RPGPlayerState|Item", Replicated)
	TArray<ARPGEquipableItem*>EquippedItems;

	/** Checks if the Item can be equipped or not on the Player. */
	bool CanEquipItem(ARPGEquipableItem* ItemToCheck);


	/** Adds the @param ItemToAdd to the Inventory*/
	void AddItemToInventory(ARPGEquipableItem* ItemToAdd);


	

	
};
