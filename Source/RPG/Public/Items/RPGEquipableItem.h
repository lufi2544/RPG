// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGEquipableItem.h"
#include "GameFramework/Actor.h"
#include "RPG/Interfaces/ItemInterface.h"
#include "RPG/Interfaces/WeaponInterface.h"

#include "RPGEquipableItem.generated.h"
class ARPGEquipableItem;

UENUM(BlueprintType)
enum class ERPGItemType : uint8
{
	//BackPack
	BackPack,

	//Weapon
	Weapon,

	//Shield
	Shield
};

USTRUCT(BlueprintType)
struct  FRPGItemData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	TSubclassOf<ARPGEquipableItem>ItemClass;

    //The Item Type.
    UPROPERTY()
    ERPGItemType ItemType;

	//The Item Name.
	UPROPERTY()
    FName ItemName;

	//Item Image
	UPROPERTY()
	UTexture2D* ItemImage;

	//If the Item Can be consume by the player(potion , food , etc).
	UPROPERTY()
	uint32 bIsConsumible :1;

	// The Damage amount that the item has.
	UPROPERTY()
    float Damage;

	//The Armor that the Item has.
	UPROPERTY()
    float Armor;

	// How much the item costs.
	UPROPERTY()
    float Price;

};

/**
 * This is the class for all the Items that Can Be Equipped by the Player
 */
UCLASS()
class ARPGEquipableItem : public AActor 
{
	GENERATED_BODY()
	
public:

	// Sets default values for this actor's properties
	ARPGEquipableItem();

	UFUNCTION(BlueprintPure , Category = "RPG|Item")
	FRPGItemData GetItemData() const;

	// Function that sets the Item to a Is Equipped State when is attatched to the Player.
	UFUNCTION(BlueprintCallable, Category = "RPG|Item")
	void SetIsEquipped(bool bNewState);

protected:

	//The Item Data
	UPROPERTY(EditAnywhere ,BlueprintReadWrite, Category = "RPG|Item")
	FRPGItemData ItemData;

	UPROPERTY(BlueprintReadWrite , Category="RPG|Item")
	uint32 bIsEquipped : 1;


};
