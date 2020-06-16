// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGWeaponComponent.h"
#include "GameFramework/Actor.h"
#include "Items/RPGEquipableItem.h"
#include "RPG/Interfaces/WeaponInterface.h"
#include "UObject/ObjectMacros.h"



#include "RPGWeapon.generated.h"

class ARPGEquipableItem;
class URPGSwordComponent;




UENUM(BlueprintType)
enum class ERPGWeaponType : uint8
{
	//None
	None,
	//Sword
	Sword,

	//Axe
	Axe,

	//Warlord
	DoubleHandSword,

	//Elf
	Bow,

	//Imperior
	Wand
};
UCLASS()
class RPG_API ARPGWeapon : public ARPGEquipableItem 
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARPGWeapon(const FObjectInitializer& ObjectInitializer);

	//Returns the Weapon type of the actual Character Weapon. 
	UFUNCTION(BlueprintPure , Category ="RPG|Weapon")
	ERPGWeaponType GetWeaponType() const;





protected:

	

	//We set the collision for the Weapon to be ready to use by the Player.
	void SetInHandCollision(URPGSwordComponent* Weapon);

	//This is the Weapon Component, it is basically a Skeletal Mesh Component.
	UPROPERTY(BlueprintReadOnly , VisibleAnywhere, Category = "RPG")
	URPGWeaponComponent* WeaponComponent;


	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = "RPG|Item|Weapon")
	ERPGWeaponType WeaponType;

};
