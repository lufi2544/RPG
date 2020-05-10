// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/RPGEquipableItem.h"
#include "RPGBag.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ARPGBag : public ARPGEquipableItem
{
	GENERATED_BODY()

	public:
	
	ARPGBag();

	
	protected:

	//The Maximum amount of objects that the Player can carry in this bag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "RPG|Item|Bag")
	int32 BagCapacity;

	//The Actual amount of objects that the Player has in this bag;
	UPROPERTY(BlueprintReadOnly, Category  = "RPG|Item|Bag")
	int32 ActuaBagCapacity;
	
};
