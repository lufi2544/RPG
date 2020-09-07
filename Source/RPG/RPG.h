// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "UObject/ObjectMacros.h"

#define ACTOR_ROLE_FSTRING *(FindObject<UEnum>(ANY_PACKAGE, TEXT("ENetRole"), true)->GetNameStringByValue(GetLocalRole()))
#define GET_ACTOR_ROLE_FSTRING(Actor) *(FindObject<UEnum>(ANY_PACKAGE, TEXT("ENetRole"), true)->GetNameStringByValue(Actor->GetLocalRole()))

RPG_API DECLARE_LOG_CATEGORY_EXTERN(RPGLog, Warning, All);

UENUM(BlueprintType)
enum class ERPGTeam : uint8
{

    Neutral,

    Ally,

    Enemy
	
};


UENUM(BlueprintType)
enum class ERPGHitReactDirection : uint8
{
    // 0
    None			UMETA(DisplayName = "None"),
    // 1
    Left 			UMETA(DisplayName = "Left"),
    // 2
    Front 			UMETA(DisplayName = "Front"),
    // 3
    Right			UMETA(DisplayName = "Right"),
    // 4
    Back			UMETA(DisplayName = "Back")
};

UENUM(BlueprintType)
enum class ERPGbilityInputID : uint8
{
    // 0 None
    None			UMETA(DisplayName = "None"),
    // 1 Confirm
    Confirm			UMETA(DisplayName = "Confirm"),
    // 2 Cancel
    Cancel			UMETA(DisplayName = "Cancel"),
    // 3 LMB
    PrimaryWeaponAttack	UMETA(DisplayName = "PrimaryWeaponAttack"),
    //4 RMB
    SecundaryWeaponAttack UMETA(DisplayName = "SecundaryWeaponAttack"),
    // 5 E
    Ability2		UMETA(DisplayName = "Ability2"),
    // 6 Q
    Ability3		UMETA(DisplayName = "Ability3"),
    // 7 
    Ability4		UMETA(DisplayName = "Ability4"),
    // 8 
    Ability5		UMETA(DisplayName = "Ability5"),
    // 9 Sprint
    Sprint			UMETA(DisplayName = "Sprint"),
    // 9 Jump
    Jump			UMETA(DisplayName = "Jump"),
    //10 BattleMode
    BattleMode     UMETA(DisplayName ="LockMode"),
    //11 Roll
    Roll            UMETA(DsiplayName = "Roll"),
    //12 Interact
    Interact           UMETA(DisplayName = "Interact"),
    //13 Block
    Block           UMETA(DisplayName = "Block")
};

/// This Enum will store all the possibles combinations of the Player animations, depending on the weappns taht he carries.
UENUM(BlueprintType)
enum class ERPGAnimationMode : uint8
{
    //No Weapon
    NoWeapon,
    //SingleSword
    SingleSword,
    //Indrax
    DoubleSword,
    //Sword and Shield
    SwordShield     UMETA(DisplayName = "Sword and Shield"),
    //Kevalam
    AxeAndShield,
    //DoubleHand Sword
    DoubleHandSword,
    //Magic wand
    MagicWand,
    //Elf
    Bow
    

    
};

UENUM(BlueprintType)
enum class ERPGCharacterHeroType : uint8
{
    //No Weapon
    NoWeapon,

    //Sword and Shield
    Spinder,

    //Axe and Shield
    Kevalam,

    //Double Sword
    Indrax,

    //Double Hand Sword
    Warlord,

    //Wizard
    Imperior,

    //Elf
    Elf
};


UENUM(BlueprintType)
enum class ERPGHitType : uint8
{
    PlayerWasHit,

    EnemyWasHit
    
};
