// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/Weapons/RPGWeapon.h"
#include "RPG/Public/Items/Weapons/RPGWeapon.h"



#include "Engine/SkeletalMesh.h"
#include "Engine/StaticMesh.h"



// Sets default values
ARPGWeapon::ARPGWeapon(const FObjectInitializer& ObjectInitializer) 
{
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//We need to replicate the Weapons to all the autonomus proxies
	SetReplicates(true);
	SetReplicateMovement(true);
	bAlwaysRelevant = true;

	




	
}

ERPGWeaponType ARPGWeapon::GetWeaponType() const
{
	return WeaponType;
}






