// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UObject/ObjectMacros.h"


#include "WeaponInterface.generated.h"

class ARPGWeapon;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RPG_API IWeaponInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/** Function used for every weapon that returns us the weapon that we grab. */
	UFUNCTION(BlueprintNativeEvent, Category = "RPG|Interfaces|Item")
	 void  GrabWeapon();
};
