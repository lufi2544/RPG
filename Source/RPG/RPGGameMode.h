// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/RPGPlayerInventoryComponent.h"
#include "GameFramework/GameModeBase.h"
#include "RPGGameMode.generated.h"

class ARPGPlayerController;

UENUM(BlueprintType)
enum class ERPGMapTravelState : uint8
{
	//Battle
	Battle,

	//This is the World that the Player belongs to in first place.
	NormalWorld
};




/** This is the Base Class for the GameMode of the RPGGame. This GameMode Can be Subclassed.
 *
 *Normally this class is going to be subclassed to be used when changing the Map.
 *
 */
UCLASS(minimalapi)
class ARPGGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	ARPGGameMode();

	/** Battle GeneralMethods Methods */



	//TODO Add an ERPGBAttleInit Enum to dictate who started the battle.
	
	/** Function that will manage the Player Travel between Levels. */
	UFUNCTION(BlueprintCallable , Category= "RPG|GameMode")
	void RPGGameModeStartBattle(ARPGPlayerController* PlayerController,ERPGMapTravelState MapTravelState);

	


};



