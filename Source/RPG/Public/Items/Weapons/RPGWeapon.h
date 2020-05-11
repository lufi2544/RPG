// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	//DoubleHandSword
	DoubleHandSword,

	//Bow
	Bow,

	//Wand
	Wand
};
UCLASS()
class RPG_API ARPGWeapon : public ARPGEquipableItem 
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARPGWeapon(const FObjectInitializer& ObjectInitializer);

	ERPGWeaponType GetWeaponType() const;





protected:

	

	//We set the collision for the Weapon to be ready to use by the Player.
	void SetInHandCollision(URPGSwordComponent* Weapon);


UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = "RPG|Item|Weapon")
	ERPGWeaponType WeaponType;

};
