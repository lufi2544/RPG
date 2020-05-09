// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG/Public/Items/RPGEquipableItem.h"



// Sets default values
ARPGEquipableItem::ARPGEquipableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SetReplicates(true);
	bReplicateMovement = true;

}

FRPGItemData ARPGEquipableItem::GetItemData() const
{
	return ItemData;
}

void ARPGEquipableItem::SetIsEquipped(bool bNewState)
{
	bIsEquipped = bNewState;
}

// Called when the game starts or when spawned


