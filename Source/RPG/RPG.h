// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "UObject/ObjectMacros.h"

#define ACTOR_ROLE_FSTRING *(FindObject<UEnum>(ANY_PACKAGE, TEXT("ENetRole"), true)->GetNameStringByValue(GetLocalRole()))
#define GET_ACTOR_ROLE_FSTRING(Actor) *(FindObject<UEnum>(ANY_PACKAGE, TEXT("ENetRole"), true)->GetNameStringByValue(Actor->GetLocalRole()))

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
    Jump			UMETA(DisplayName = "Jump")
    //10 ZoomMode
    ,LockMode UMETA(DisplayName ="LockMode")
};