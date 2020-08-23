// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "RPGGameMode.h"
#include "UObject/ConstructorHelpers.h"

ARPGGameMode::ARPGGameMode()
{


}

void ARPGGameMode::TravelPlayerToLevel(ARPGPlayerController* PlayerController,ERPGMapTravelState MapTravelState)
{
    FString MapURL = "null";
    
    if (MapTravelState == ERPGMapTravelState::Battle)
    {
        MapURL = "/Game/Maps/WorldMap.WorldMap";    
    }

    if (PlayerController && (MapURL != "null"))
    {
        
        PlayerController->ClientTravel(MapURL,ETravelType::TRAVEL_Absolute);
    }
    
}
