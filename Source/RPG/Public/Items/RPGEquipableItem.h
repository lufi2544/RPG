// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPG/Interfaces/ItemInterface.h"

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

	//For Init Pruposes
	,None
};

USTRUCT(BlueprintType)
struct  FRPGItemData
{
	GENERATED_USTRUCT_BODY()

	FRPGItemData(){}

	UPROPERTY(BlueprintReadOnly)
	TSubclassOf<ARPGEquipableItem>ItemClass;

    //The Item Type.
    UPROPERTY(BlueprintReadOnly,EditAnywhere)
    ERPGItemType ItemType = ERPGItemType::None;

	//The Item Name.
	UPROPERTY(BlueprintReadOnly,EditAnywhere)
    FName ItemName = "Name";

	//Item Image
	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	UTexture2D* ItemImage = nullptr;

	//If the Item Can be consume by the player(potion , food , etc).
	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	uint32 bIsConsumible :1;

	// The Damage amount that the item has.
	UPROPERTY(BlueprintReadOnly,EditAnywhere)
    float Damage = 0.0f;

	//The Armor that the Item has.
	UPROPERTY(BlueprintReadOnly,EditAnywhere)
    float Armor = 0.0f;

	// How much the item costs.
	UPROPERTY(BlueprintReadOnly,EditAnywhere)
    float Price = 0.0f;

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

	// Function that sets the Item to an Is Equipped State when is attatched to the Player.
	UFUNCTION(BlueprintCallable, Category = "RPG|Item")
	void SetIsEquipped(bool bNewState);

protected:

	//The Item Data
	UPROPERTY(EditAnywhere ,BlueprintReadWrite, Category = "RPG|Item")
	FRPGItemData ItemData;

	UPROPERTY(BlueprintReadWrite , Category="RPG|Item")
	uint32 bIsEquipped : 1;


};
