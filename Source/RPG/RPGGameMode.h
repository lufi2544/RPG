// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/RPGPlayerInventoryComponent.h"
#include "GameFramework/GameModeBase.h"
#include "RPGGameMode.generated.h"


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

	//TODO In the future I should Implement a new check here to Spawn the PLayer in a new Map depending on the Position of the old map.

	//TODO Add the PreclientTravel to check the PreMapVariation check.
	
	/** Function that will manage the Player Travel between Levels. */
	UFUNCTION(BlueprintCallable , Category= "RPG|GameMode")
	void TravelPlayerToLevel(ARPGPlayerController* PlayerController,ERPGMapTravelState MapTravelState); 


};



